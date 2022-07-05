
#ifndef ANSWERWIDGET_H
#define ANSWERWIDGET_H

#include "Examinator.h"
#include "RoundedGradientWidget.h"

/*!
 * \class AnswerWidget
 */
class AnswerWidget : public RoundedGradientWidget
{
Q_OBJECT
public:
    AnswerWidget(QWidget * parent = 0);
    virtual ~AnswerWidget();

    QString answer();

public slots:
    void setAnswer(QString);
    void setExaminatorMode(Examinator::Mode);
    void setExaminatorState(Examinator::State);

signals:
    void stop();
    void userAnswerChanged(QString);
    void commitAnswer(QString);
    void userEvent(int msecs, int eventType);

private slots:
    void textChanged(const QString &);
    void returnPressed();
    void escapePressed();

protected:
    void paintEvent(QPaintEvent *);

private:
    class Private;
    Private * d;
};

#include <QLineEdit>

class AnswerLineEdit : public QLineEdit
{
Q_OBJECT
public:
    enum EventType {
        TypingStarted = 0,
        SymbolTyped
    };
    AnswerLineEdit(QWidget * parent = 0);
    virtual ~AnswerLineEdit();

public slots:
    void reset();

signals:
    void escapePressed();
    void userEvent(int msecs, int eventType);

protected:
    virtual void keyPressEvent(QKeyEvent * ke);

private:
    class Private;
    Private * d;
};

#endif // ANSWERWIDGET_H
