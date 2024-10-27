#ifndef PERSON_H
#define PERSON_H

#include <QString>

class Person {
public:
    Person(const QString &name, int ticket_number)
        : name(name), ticket_number(ticket_number) {}

    QString name;
    int ticket_number;
};

#endif // PERSON_H

