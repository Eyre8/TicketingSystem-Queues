#include "queue.h"

Queue::Queue(QObject *parent) : QObject(parent) {}

void Queue::enqueue(const QString &name) {
    Person new_person(name, next_ticket_number++);
    queue.enqueue(new_person);
    emit queueUpdated();
}

Person Queue::dequeue() {
    if (!queue.isEmpty()) {
        Person removedPerson = queue.dequeue();
        emit queueUpdated();           // Emit signal after dequeuing
        emit personDequeued(removedPerson);  // Emit personDequeued signal
        return removedPerson;
    }
    return Person("", -1);  // Return a default empty person if queue is empty
}



bool Queue::isEmpty() {
    return queue.isEmpty();
}

int Queue::size() {
    return queue.size();
}

QList<Person> Queue::getQueueList() {
    return QList<Person>(queue.begin(), queue.end());
}
