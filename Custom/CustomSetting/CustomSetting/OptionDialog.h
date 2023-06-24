#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDir>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QScrollArea>
#include <QSpinBox>
#include <QStackedWidget>
#include <QTreeWidget>
class QSettings;
class GeneralSettingsPage;
class LibrariesPage;
class OptionDialog : public QDialog
{
    Q_OBJECT

private:
    OptionDialog(QWidget* parent = nullptr);
    void setUpDialog();
    void addListItems();
    void createPages();
    void addPage(QWidget* pPage);
    void saveDialogGeometry();

public:
    ~OptionDialog();
    static OptionDialog* instance()
    {
        static OptionDialog* pInstance = new OptionDialog;
        return pInstance;
    }
public slots:
    void changePage(QListWidgetItem* current, QListWidgetItem* previous);
    void reject() override;
    void saveSettings();
    void reset();

private:
    QSettings*           mpSettings;
    GeneralSettingsPage* mpGeneralSettingsPage;
    LibrariesPage*       mpLibrariesPage;
    QListWidget*         mpOptionsList;
    QStackedWidget*      mpPagesWidget;
    QLabel*              mpChangesEffectLabel;
    QPushButton*         mpCancelButton;
    QPushButton*         mpOkButton;
    QPushButton*         mpResetButton;
    QDialogButtonBox*    mpButtonBox;
};

class GeneralSettingsPage : public QWidget
{
    Q_OBJECT
public:
    enum AccessAnnotations
    {
        Always = 0,
        Loading = 1,
        Never = 2
    };
    GeneralSettingsPage(OptionDialog* pOptionsDialog);

private:
    void setToolTip(QComboBox* pComboBox, const QString& description, const QStringList& optionsDescriptions);

private:
    OptionDialog* mpOptionsDialog;
    QGroupBox*    mpGeneralSettingsGroupBox;
    QLabel*       mpLanguageLabel;
    QComboBox*    mpLanguageComboBox;
    QLabel*       mpWorkingDirectoryLabel;
    QLineEdit*    mpWorkingDirectoryTextBox;
    QPushButton*  mpWorkingDirectoryBrowseButton;
    QLabel*       mpToolbarIconSizeLabel;
    QSpinBox*     mpToolbarIconSizeSpinBox;
    QCheckBox*    mpPreserveUserCustomizations;
    QLabel*       mpTerminalCommandLabel;
    QLineEdit*    mpTerminalCommandTextBox;
    QPushButton*  mpTerminalCommandBrowseButton;
    QLabel*       mpTerminalCommandArgumentsLabel;
    QLineEdit*    mpTerminalCommandArgumentsTextBox;
    QCheckBox*    mpHideVariablesBrowserCheckBox;
    QLabel*       mpActivateAccessAnnotationsLabel;
    QComboBox*    mpActivateAccessAnnotationsComboBox;
    QCheckBox*    mpCreateBackupFileCheckbox;
    QCheckBox*    mpDisplayNFAPIErrorsWarningsCheckBox;
    QGroupBox*    mpLibrariesBrowserGroupBox;
    QLabel*       mpLibraryIconSizeLabel;
    QSpinBox*     mpLibraryIconSizeSpinBox;
    QLabel*       mpLibraryIconTextLengthLabel;
    QSpinBox*     mpLibraryIconTextLengthSpinBox;
    QCheckBox*    mpShowProtectedClasses;
    QCheckBox*    mpShowHiddenClasses;
    QCheckBox*    mpSynchronizeWithModelWidgetCheckBox;
    QGroupBox*    mpEnableAutoSaveGroupBox;
    QLabel*       mpAutoSaveIntervalLabel;
    QSpinBox*     mpAutoSaveIntervalSpinBox;
    QLabel*       mpAutoSaveSecondsLabel;
    QGroupBox*    mpWelcomePageGroupBox;
    QRadioButton* mpHorizontalViewRadioButton;
    QRadioButton* mpVerticalViewRadioButton;
    QCheckBox*    mpShowLatestNewsCheckBox;
    QSpinBox*     mpRecentFilesAndLatestNewsSizeSpinBox;
    QGroupBox*    mpOptionalFeaturesGroupBox;
    QCheckBox*    mpEnableInstanceAPICheckBox;
public slots:
    void selectWorkingDirectory();
    //    void selectTerminalCommand();
    void autoSaveIntervalValueChanged(int value);
};

class LibrariesPage : public QWidget
{
    Q_OBJECT
public:
    LibrariesPage(OptionDialog* pOptionsDialog);

private:
    OptionDialog*     mpOptionsDialog;
    QGroupBox*        mpSystemLibrariesGroupBox;
    QLabel*           mpModelicaPathLabel;
    QLineEdit*        mpModelicaPathTextBox;
    QPushButton*      mpModelicaPathBrowseButton;
    QLabel*           mpSystemLibrariesNoteLabel;
    QCheckBox*        mpLoadLatestModelicaCheckbox;
    QTreeWidget*      mpSystemLibrariesTree;
    QPushButton*      mpAddSystemLibraryButton;
    QPushButton*      mpRemoveSystemLibraryButton;
    QPushButton*      mpEditSystemLibraryButton;
    QDialogButtonBox* mpSystemLibrariesButtonBox;
    QGroupBox*        mpUserLibrariesGroupBox;
    QTreeWidget*      mpUserLibrariesTree;
    QPushButton*      mpAddUserLibraryButton;
    QPushButton*      mpRemoveUserLibraryButton;
    QPushButton*      mpEditUserLibraryButton;
    QDialogButtonBox* mpUserLibrariesButtonBox;
private slots:
    void selectModelicaPath();
    void openAddSystemLibrary();
    void removeSystemLibrary();
    void openEditSystemLibrary();
    void openAddUserLibrary();
    void removeUserLibrary();
    void openEditUserLibrary();
};

class AddSystemLibraryDialog : public QDialog
{
    Q_OBJECT
public:
    AddSystemLibraryDialog(LibrariesPage* pLibrariesPage, bool editFlag = false);
    bool       nameExists(QTreeWidgetItem* pItem = 0);
    QComboBox* getNameComboBox() const
    {
        return mpNameComboBox;
    }
    QComboBox* getVersionsComboBox() const
    {
        return mpVersionsComboBox;
    }

    void getSystemLibraries();

private:
    LibrariesPage*    mpLibrariesPage;
    QLabel*           mpNameLabel;
    QComboBox*        mpNameComboBox;
    QLabel*           mpValueLabel;
    QComboBox*        mpVersionsComboBox;
    QPushButton*      mpOkButton;
    QPushButton*      mpCancelButton;
    QPushButton*      mpInstallLibraryButton;
    QDialogButtonBox* mpButtonBox;
    bool              mEditFlag;

private slots:
    void getLibraryVersions(const QString& library);
    void addSystemLibrary();
    void openInstallLibraryDialog();
};

#endif // OPTIONDIALOG_H
