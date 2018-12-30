/*
 * This file is part of BeamerPresent.
 *
 * BeamerPresent is free and unencumbered public domain software.
 * For more information, see http://unlicense.org/ or the accompanying
 * UNLICENSE file.
 */

#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <QLabel>
#include <QLineEdit>
#include <QTime>
#include <QTimer>

class Timer : public QLabel
{
    Q_OBJECT

public:
    Timer(QWidget * parent = nullptr);
    Timer(QLineEdit * setTimerEdit, QWidget * parent = nullptr);
    ~Timer();
    void setTimerWidget(QLineEdit * setTimerEdit);

public slots:
    void setDeadline();
    void pauseTimer();
    void resetTimer();
    void continueTimer();

private slots:
    void showTime();

signals:
    void sendAlert();
    void sendNoAlert();
    void sendEscape();

private:
    QLineEdit * timerEdit;
    QTime * deadline;
    QTime * time;
    QTimer * timer;
    bool running = false;
    const int colorTimeInterval = 150;
};

#endif // TIMER_H
