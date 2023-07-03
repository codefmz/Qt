#ifndef FIXEDCHECKBOX_H
#define FIXEDCHECKBOX_H

#include <QCheckBox>

class FixedCheckBox : public QCheckBox
{
    Q_OBJECT
private:
    bool mDefaultValue;
    bool mInheritedValue;
    bool mFixedState;

public:
    FixedCheckBox(QWidget* parent = 0);
    void setTickState(bool defaultValue, bool fixedState);
    bool isDefaultValue()
    {
        return mDefaultValue;
    }
    bool getInheritedValue() const
    {
        return mInheritedValue;
    }
    QString getTickStateString() const;

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // FIXEDCHECKBOX_H
