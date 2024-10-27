#ifndef QUEUE_H
#define QUEUE_H

#include <QObject>
#include <QQueue>
#include "person.h"

class Queue : public QObject {
    Q_OBJECT

public:
    explicit Queue(QObject *parent = nullptr);
    void enqueue(const QString &name);
    Person dequeue();
    bool isEmpty();
    int size();
    QList<Person> getQueueList();

signals:
    void queueUpdated();
    void personDequeued(const Person &dequeuedPerson);

private:
    QQueue<Person> queue;
    int next_ticket_number = 1;
};

#endif // QUEUE_H

