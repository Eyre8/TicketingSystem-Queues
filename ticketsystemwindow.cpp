#include "ticketsystemwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>

TicketSystemWindow::TicketSystemWindow(QWidget *parent)
    : QMainWindow(parent), timeRemaining(60), isDequeueInProgress(false) {
    this->setFixedSize(800, 700);
    countdownTimer = new QTimer(this);
    QPixmap background(":/images/bg.png");
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(background.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    this->setPalette(palette);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Left side - Queue visualization
    queueList = new QListWidget(this);
    queueList->setStyleSheet("QListWidget {"
                             "background-image: url(:/images/bg2.jpeg);"
                             "background-color: #297abd;"
                             "background-repeat: no-repeat;"
                             "background-position: center;"
                             "font-size: 20px;"
                             "font-weight: bold;"
                             "border-radius: 8px;"
                             "border: 4px solid #480b6e;"
                             "}");
    timerLabel = new QLabel("Next dequeue in 60 seconds", this);
    timerLabel->setAlignment(Qt::AlignCenter);
    timerLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #ffffff; background-color: #a30cff; padding: 10px; border: 4px solid #480b6e; border-radius: 10px;");

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(queueList);
    leftLayout->addWidget(timerLabel);

    // Right side - Command panel
    QVBoxLayout *rightLayout = new QVBoxLayout();

    QLabel *titleLabel = new QLabel("Olivia Rodrigo Live Concert", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFixedSize(300, 150);
    titleLabel->setStyleSheet("font-size: 23px; font-weight: bold; color: #462958;");
    rightLayout->addWidget(titleLabel);

    QWidget *inputWidget = new QWidget(this);
    inputWidget->setFixedSize(300, 150);
    inputWidget->setStyleSheet("background-color: white; border-radius: 10px; border: 4px solid #480b6e;");

    QVBoxLayout *inputLayout = new QVBoxLayout(inputWidget);

    nameInput = new QLineEdit(this);
    nameInput->setPlaceholderText("Enter person's name");
    nameInput->setStyleSheet("font-size:16px; color:#480b6e; border: 2px solid #480b6e; padding: 3px; font-weight: bold;");
    inputLayout->addWidget(nameInput);
    QPushButton *addButton = new QPushButton("Add to Queue", this);
    addButton->setStyleSheet( "QPushButton {"
                             "font-size:16px;"
                            "color: white;"
                            "background-color: #480b6e;"
                            "padding: 3px;" ""
                             "font-weight: bold;"
                             "}"
                             "QPushButton:hover {"
                             "    background-color: #9418e2;"
                             "    color: #000000;"
                            "}");
    inputLayout->addWidget(addButton);

    rightLayout->addWidget(inputWidget);

    // Dequeued message label
    dequeueMessageLabel = new QLabel(this);
    rightLayout->addWidget(dequeueMessageLabel);

    dequeueMessageLabel->setText("");

    // Split layout for both sides
    QHBoxLayout *splitLayout = new QHBoxLayout();
    splitLayout->addLayout(leftLayout);
    splitLayout->addLayout(rightLayout);

    mainLayout->addLayout(splitLayout);
    setCentralWidget(centralWidget);

    // Connect the button to the queue logic
    connect(addButton, &QPushButton::clicked, this, &TicketSystemWindow::onAddToQueue);

    // Connect the queue's update signal to the display update slot
    connect(&queue, &Queue::queueUpdated, this, &TicketSystemWindow::updateQueueDisplay);

    // Connect the countdown timer to the updateCountdown slot
    connect(countdownTimer, &QTimer::timeout, this, &TicketSystemWindow::updateCountdown);

    connect(&queue, &Queue::personDequeued, this, &TicketSystemWindow::onPersonDequeued);

}

void TicketSystemWindow::onAddToQueue() {
    QString name = nameInput->text();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a name.");
        return;
    }

    queue.enqueue(name);
    nameInput->clear();

    // Start the countdown timer if the queue was previously empty
    if (queue.size() == 1) {
        timeRemaining = 60;  // Reset the countdown to 60
        countdownTimer->start(1000);  // Start the timer to trigger every second
    }
}

void TicketSystemWindow::updateQueueDisplay() {
    queueList->clear();
    QList<Person> personList = queue.getQueueList();
    for (const Person &p : personList) {
        QListWidgetItem *item = new QListWidgetItem(p.name + " (Ticket #" + QString::number(p.ticket_number) + ")");

        item->setBackground(QColor(221, 160, 221, 118));
        item->setForeground(Qt::white);

        queueList->addItem(item);
    }
}

void TicketSystemWindow::updateCountdown() {
    if (timeRemaining > 0) {
        timeRemaining--;
        timerLabel->setText("Next dequeue in " + QString::number(timeRemaining) + " seconds");
    }

    // When the timer hits zero
    if (timeRemaining == 0 && !isDequeueInProgress) {
        isDequeueInProgress = true;
        countdownTimer->stop();

        // Debugging: Log before dequeuing
        qDebug() << "Dequeue triggered, calling queue.dequeue()";

        // Perform the dequeue and check if it's successful
        if (!queue.isEmpty()) {
            queue.dequeue();  // Remove the person at the front of the queue
            qDebug() << "Person dequeued, queue size: " << queue.size();
        } else {
            qDebug() << "Queue is empty, no dequeue performed.";
        }

        // Reset countdown and UI display
        timeRemaining = 60;
        timerLabel->setText("Next dequeue in 60 seconds");

        // Restart the timer after 1 second, ensuring proper spacing between dequeues
        QTimer::singleShot(1000, this, [this]() {
            countdownTimer->start(1000);
            isDequeueInProgress = false;
        });
    }
}

void TicketSystemWindow::onPersonDequeued(const Person &dequeuedPerson) {
    if (dequeuedPerson.name != "") {  // Check if a valid person was dequeued
        QString message = QString("%1 (Ticket #%2) has been dequeued")
                              .arg(dequeuedPerson.name)
                              .arg(dequeuedPerson.ticket_number);
        dequeueMessageLabel->setText(message);
        dequeueMessageLabel->setFixedSize(300, 150);
        dequeueMessageLabel->setStyleSheet("font-size: 12px; font-weight: bold; color: #ffffff; background-color: #a30cff; padding: 10px; border: 4px solid #480b6e; border-radius: 10px;");
        dequeueMessageLabel->setAlignment(Qt::AlignCenter);
    }
}


