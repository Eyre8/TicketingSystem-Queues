#ifndef TICKETSYSTEMWINDOW_H
#define TICKETSYSTEMWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QListWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include "queue.h"

class TicketSystemWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit TicketSystemWindow(QWidget *parent = nullptr);

private slots:
    void onAddToQueue();
    void updateQueueDisplay();
    void updateCountdown();
    void onPersonDequeued(const Person &dequeuedPerson);

private:
    QLabel *dequeueMessageLabel;
    Queue queue;
    QTimer *countdownTimer;
    int timeRemaining;
    bool isDequeueInProgress;

    QListWidget *queueList;
    QLabel *timerLabel;
    QLineEdit *nameInput;
};

#endif // TICKETSYSTEMWINDOW_H
