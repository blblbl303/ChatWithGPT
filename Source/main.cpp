#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QStyleFactory>
#include <QtWidgets/QPlainTextEdit>
#include "liboai.h"
#include <string>
#include <QWidget>
#include <QPixmap>
#include <iostream>
#include <sqlite3.h>
#include <sstream>

using namespace liboai;

OpenAI oai;
// create a conversation
Conversation convo;

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Открываем базу данных
    sqlite3* db;
    int rc = sqlite3_open("test.db", &db);

    if (rc != SQLITE_OK) {
        std::cerr << "Не удалось открыть базу данных: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    std::cout << "База данных успешно открыта" << std::endl;

    // Создаем таблицу, если она не существует
    const char* sql = "CREATE TABLE IF NOT EXISTS users (name TEXT)";
    char* errMsg;
    rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "Не удалось создать таблицу: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    std::cout << "Таблица успешно создана или уже существует" << std::endl;

    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    QApplication::setPalette(darkPalette);

    // Главное окно
    QMainWindow window;
    window.setWindowTitle("ChatWithGPT");
    window.QMainWindow::resize(1000, 650);

    // Приветствие
    QLabel* label = new QLabel("Ask your question here:");
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-size: 20px;");

    // Поле для ввода вопроса
    QLineEdit* questionField = new QLineEdit();
    questionField->setStyleSheet("font-size: 16px;");
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(1);
    questionField->setSizePolicy(sizePolicy);
    questionField->setFixedHeight(40);

    // Поле для вывода ответа
    QPlainTextEdit* answerField = new QPlainTextEdit();
    answerField->setReadOnly(true);
    answerField->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    answerField->setStyleSheet("font-size: 16px;");

    // Выбираем данные из таблицы
    sql = "SELECT name FROM users";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Не удалось выполнить запрос: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    // Получаем результаты запроса
    std::cout << "Имена пользователей:" << std::endl;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::cout << "- " << name << std::endl;
        QString qname = QString::fromStdString(name);
        answerField->appendPlainText("History: " + qname + "\n");
    }

    // Освобождаем ресурсы
    sqlite3_finalize(stmt);
    // Закрываем базу данных
    sqlite3_close(db);

    // Кнопка
    QPushButton* sendButton = new QPushButton("Picture generation");
    sendButton->setStyleSheet("font-size: 16px;");
    sizePolicy.setHorizontalStretch(0);
    sendButton->setSizePolicy(sizePolicy);
    sendButton->setFixedHeight(40);
    QObject::connect(sendButton, &QPushButton::clicked, [&]() {

        QString question = questionField->text();

        QString filePath = "C:\\Users\\User\\Desktop\\image\\file.png";

        answerField->appendPlainText("You: " + question + "\n");
        std::string str = question.toStdString();
        OpenAI oai;
        if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
            try {
                Response response = oai.Image->create(
                    str,
                    1,
                    "512x512"
                );
                Network::Download(filePath.toStdString(), response["data"][0]["url"].get<std::string>(), {});

            }
            catch (std::exception& e) {
                std::cout << e.what() << std::endl;
            }
        }

        // Создаем окно
        QWidget* window = new QWidget();
        window->setWindowTitle(question);

        // Создаем метку и устанавливаем в нее текст
        QLabel* label = new QLabel();
        //label->setText("The picture is created...");

        QPixmap pixmap(filePath);
        if (!pixmap.isNull()) {
            label->setPixmap(pixmap);
            window->setFixedSize(pixmap.size());
        }
        else {
            label->setText("2");
        }
        // Создаем layout и добавляем метку в него
        QVBoxLayout* layout = new QVBoxLayout(window);
        layout->addWidget(label);

        // Отображаем окно
        window->show();

        answerField->appendPlainText("DELL-E: " + question + "\n\n");

        questionField->clear();
        });

    // Enter
    QObject::connect(questionField, &QLineEdit::returnPressed, [&]() {

        QString question = questionField->text();

        answerField->appendPlainText("You: " + question + "\n");
        std::string str = question.toStdString();

        convo.AddUserData(str);
        if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
            try {
                Response response = oai.ChatCompletion->create(
                    "gpt-3.5-turbo", convo
                );

                // update our conversation with the response
                convo.Update(response);
                std::cout << convo.GetLastResponse() << std::endl;
                QString qstr = QString::fromStdString(convo.GetLastResponse());
                // print the response
                answerField->appendPlainText("ChatGPT:" + qstr + ".\n\n");

                // Открываем базу данных
                sqlite3* db;
                int rc = sqlite3_open("test.db", &db);

                if (rc != SQLITE_OK) {
                    std::cerr << "Не удалось открыть базу данных: " << sqlite3_errmsg(db) << std::endl;
                    sqlite3_close(db);
                    return 1;
                }

                std::cout << "База данных успешно открыта" << std::endl;

                // Создаем таблицу, если она не существует
                const char* sql = "CREATE TABLE IF NOT EXISTS users (name TEXT)";
                char* errMsg;
                rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);

                if (rc != SQLITE_OK) {
                    std::cerr << "Не удалось создать таблицу: " << errMsg << std::endl;
                    sqlite3_free(errMsg);
                    sqlite3_close(db);
                    return 1;
                }

                std::cout << "Таблица успешно создана или уже существует" << std::endl;

                // Добавляем данные в таблицу
                std::string strname = qstr.toStdString();
               
                std::stringstream ss;
                ss << "INSERT INTO users (name) VALUES ('" << str << "'), ('" << strname << "') ";
                std::string sql_str = ss.str(); // Исправлено
                const char* sql_cstr = sql_str.c_str();
                rc = sqlite3_exec(db, sql_cstr, nullptr, nullptr, &errMsg);

                if (rc != SQLITE_OK) {
                    std::cerr << "Не удалось записать данные в таблицу: " << errMsg << std::endl;
                    sqlite3_free(errMsg);
                    sqlite3_close(db);
                    return 1;
                }
                std::cout << "Данные успешно записаны в таблицу 7" << std::endl;

                // Закрываем базу данных
                sqlite3_close(db);
            }
            catch (std::exception& e) {
                std::cout << e.what() << std::endl;
            }
        }
        questionField->clear();
        });

    // Создаем горизонтальную компоновку для текстового поля и кнопки
    QHBoxLayout* inputLayout = new QHBoxLayout();
    inputLayout->addWidget(questionField);
    inputLayout->addWidget(sendButton);

    // Создаем вертикальную компоновку для метки, текстовых полей и горизонтальной компоновки
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(label);
    mainLayout->addWidget(answerField);
    mainLayout->addLayout(inputLayout);

    // Создаем центральный виджет и устанавливаем на него главную компоновку
    QWidget* centralWidget = new QWidget();
    centralWidget->setLayout(mainLayout);
    window.setCentralWidget(centralWidget);

    window.show();

    return app.exec();
}