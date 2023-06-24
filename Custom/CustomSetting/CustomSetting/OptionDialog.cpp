#include "OptionDialog.h"
#include "Utilities.h"
#include <QMessageBox>
#include <QScrollBar>
OptionDialog::OptionDialog(QWidget* parent)
    : QDialog(parent)
    , mpSettings(Utilities::getApplicationSettings())
{

    setWindowTitle(QString("%1 - %2").arg("Custom", "CustomSetting"));
    setModal(true); //执行show 是模态的
    mpGeneralSettingsPage = new GeneralSettingsPage(this);
    mpLibrariesPage = new LibrariesPage(this);
    setUpDialog();
}

void OptionDialog::setUpDialog()
{
    mpOptionsList = new QListWidget;
    //    mpOptionsList->setItemDelegate(new ItemDelegate(mpOptionsList));
    mpOptionsList->setViewMode(QListView::ListMode);
    mpOptionsList->setMovement(QListView::Static);
    mpOptionsList->setIconSize(QSize(24, 24));
    mpOptionsList->setCurrentRow(0, QItemSelectionModel::Select);
    connect(mpOptionsList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), SLOT(changePage(QListWidgetItem*, QListWidgetItem*)));
    // add items to options list
    addListItems();
    // get maximum width for options list
    mpOptionsList->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    int width = mpOptionsList->sizeHintForColumn(0) + mpOptionsList->frameWidth() * 2 + 20;
    if (mpOptionsList->verticalScrollBar()->isVisible())
    {
        width += mpOptionsList->verticalScrollBar()->width();
    }
    mpOptionsList->setMaximumWidth(width);
    // create pages
    createPages();
    mpChangesEffectLabel = new QLabel(tr("* The changes will take effect after restart."));
    //    mpChangesEffectLabel->setElideMode(Qt::ElideMiddle);
    // Create the buttons
    mpOkButton = new QPushButton(tr("OK"));
    mpOkButton->setAutoDefault(true);
    connect(mpOkButton, SIGNAL(clicked()), SLOT(saveSettings()));
    mpCancelButton = new QPushButton(tr("Cancel"));
    mpCancelButton->setAutoDefault(false);
    connect(mpCancelButton, SIGNAL(clicked()), SLOT(reject()));
    mpResetButton = new QPushButton(tr("Reset"));
    mpResetButton->setAutoDefault(false);
    connect(mpResetButton, SIGNAL(clicked()), SLOT(reset()));
    mpButtonBox = new QDialogButtonBox(Qt::Horizontal);
    mpButtonBox->addButton(mpOkButton, QDialogButtonBox::ActionRole);
    mpButtonBox->addButton(mpResetButton, QDialogButtonBox::ActionRole);
    mpButtonBox->addButton(mpCancelButton, QDialogButtonBox::ActionRole);
    QHBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(mpOptionsList);
    horizontalLayout->addWidget(mpPagesWidget);
    // Create a layout
    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addLayout(horizontalLayout, 0, 0, 1, 2);
    mainLayout->addWidget(mpChangesEffectLabel, 1, 0);
    mainLayout->addWidget(mpButtonBox, 1, 1, Qt::AlignRight);
    setLayout(mainLayout);
}

void OptionDialog::addListItems()
{
    // General Settings Item
    QListWidgetItem* pGeneralSettingsItem = new QListWidgetItem(mpOptionsList);
    pGeneralSettingsItem->setIcon(QIcon(":/res/general.svg"));
    pGeneralSettingsItem->setText(tr("General"));
    mpOptionsList->item(0)->setSelected(true);
    // Libraries Item
    QListWidgetItem* pLibrariesItem = new QListWidgetItem(mpOptionsList);
    pLibrariesItem->setIcon(QIcon(":/res/libraries.svg"));
    pLibrariesItem->setText(tr("Libraries"));
}

void OptionDialog::createPages()
{
    mpPagesWidget = new QStackedWidget;
    addPage(mpGeneralSettingsPage);
    addPage(mpLibrariesPage);
}

void OptionDialog::addPage(QWidget* pPage)
{
    QScrollArea* pScrollArea = new QScrollArea;
    pScrollArea->setWidgetResizable(true);
    pScrollArea->setWidget(pPage);
    mpPagesWidget->addWidget(pScrollArea);
}

void OptionDialog::saveDialogGeometry()
{
    mpSettings->setValue("OptionsDialog/geometry", saveGeometry());
}

OptionDialog::~OptionDialog() {}

void OptionDialog::changePage(QListWidgetItem* current, QListWidgetItem* previous)
{
    if (!current)
    {
        current = previous;
    }
    mpPagesWidget->setCurrentIndex(mpOptionsList->row(current));
}

void OptionDialog::reject()
{
    saveDialogGeometry();
    QDialog::reject();
}

void OptionDialog::saveSettings()
{
    saveDialogGeometry();
    accept();
}

void OptionDialog::reset()
{
    const QString title = tr("Reset to default");
    const QString text0 = tr("Are you sure that you want to reset OMEdit? This operation cannot be undone. ");
    const QString textWithLink = tr(("Please back up your settings " + QString("<a href='%1'>file</a>").arg(mpSettings->fileName())
                                     + " before proceeding, restart to have the changes take effect.")
                                        .toUtf8()
                                        .constData());
    const QString text = text0 + textWithLink;
    QMessageBox*  pResetMessageBox = new QMessageBox();
    pResetMessageBox->setTextFormat(Qt::RichText);
    pResetMessageBox->setWindowTitle(title);
    pResetMessageBox->setText(text);
    const QMessageBox::StandardButton reply = pResetMessageBox->question(this, title, text, QMessageBox::Ok | QMessageBox::Cancel);
    if (reply == QMessageBox::Ok)
    {
        mpSettings->clear();
        mpSettings->sync();
        accept();
        destroy();
    }
    delete pResetMessageBox;
}

GeneralSettingsPage::GeneralSettingsPage(OptionDialog* pOptionsDialog)
    : QWidget(pOptionsDialog)
{
    mpOptionsDialog = pOptionsDialog;
    mpGeneralSettingsGroupBox = new QGroupBox(tr("general"));
    // Language option
    mpLanguageLabel = new QLabel(tr("Language: *"));
    mpLanguageComboBox = new QComboBox;
    mpLanguageComboBox->addItem(tr("Auto Detected"), "");
    QMap<QString, QLocale> map;
    map.insert(tr("Chinese").append(" (zh_CN)"), QLocale(QLocale::Chinese));
    map.insert(tr("English").append(" (en)"), QLocale(QLocale::English));
    map.insert(tr("French").append(" (fr)"), QLocale(QLocale::French));
    map.insert(tr("German").append(" (de)"), QLocale(QLocale::German));
    map.insert(tr("Italian").append(" (it)"), QLocale(QLocale::Italian));
    map.insert(tr("Japanese").append(" (ja)"), QLocale(QLocale::Japanese));
    map.insert(tr("Romanian").append(" (ro)"), QLocale(QLocale::Romanian));
    map.insert(tr("Russian").append(" (ru)"), QLocale(QLocale::Russian));
    map.insert(tr("Spanish").append(" (es)"), QLocale(QLocale::Spanish));
    map.insert(tr("Swedish").append(" (sv)"), QLocale(QLocale::Swedish));
    QStringList keys(map.keys());
    keys.sort();
    foreach (const QString& key, keys)
    {
        QLocale locale = map[key];
        mpLanguageComboBox->addItem(key, locale);
    }
    //    mpLanguageComboBox->installEventFilter(mpOptionsDialog);

    // Working Directory
    mpWorkingDirectoryLabel = new QLabel("working directory");
    mpWorkingDirectoryTextBox = new QLineEdit;
    mpWorkingDirectoryTextBox->setPlaceholderText(QDir::currentPath());
    mpWorkingDirectoryBrowseButton = new QPushButton(tr("browse"));
    mpWorkingDirectoryBrowseButton->setAutoDefault(false);
    connect(mpWorkingDirectoryBrowseButton, SIGNAL(clicked()), SLOT(selectWorkingDirectory()));
    // toolbar icon size
    mpToolbarIconSizeLabel = new QLabel(tr("Toolbar Icon Size: *"));
    mpToolbarIconSizeSpinBox = new QSpinBox;
    mpToolbarIconSizeSpinBox->setMinimum(16);
    mpToolbarIconSizeSpinBox->setValue(20);
    // Store Customizations Option
    mpPreserveUserCustomizations = new QCheckBox(tr("Preserve User's GUI Customizations"));
    mpPreserveUserCustomizations->setChecked(true);
    // terminal command
    mpTerminalCommandLabel = new QLabel(tr("Terminal Command:"));
    mpTerminalCommandTextBox = new QLineEdit;
    mpTerminalCommandTextBox->setText(tr("cmd.exe"));
    mpTerminalCommandBrowseButton = new QPushButton(tr("browse"));
    mpTerminalCommandBrowseButton->setAutoDefault(false);
    connect(mpTerminalCommandBrowseButton, SIGNAL(clicked()), SLOT(selectTerminalCommand()));
    // terminal command args
    mpTerminalCommandArgumentsLabel = new QLabel(tr("Terminal Command Arguments:"));
    mpTerminalCommandArgumentsTextBox = new QLineEdit;
    // hide variables browser checkbox
    mpHideVariablesBrowserCheckBox = new QCheckBox(tr("Hide Variables Browser"));
    mpHideVariablesBrowserCheckBox->setToolTip(tr("Hides the variable browser when switching away from plotting perspective."));
    mpHideVariablesBrowserCheckBox->setChecked(false);
    // activate access annotation
    mpActivateAccessAnnotationsLabel = new QLabel(tr("Activate Access Annotations *"));
    mpActivateAccessAnnotationsComboBox = new QComboBox;
    QStringList activateAccessAnnotationsDescriptions;
    activateAccessAnnotationsDescriptions << tr("Activates the access annotations even for the non-encrypted libraries.")
                                          << tr("Activates the access annotations even if the .mol contains a non-encrypted library.")
                                          << tr("Deactivates access annotations except for encrypted libraries.");
    mpActivateAccessAnnotationsComboBox->addItem(tr("Always"), GeneralSettingsPage::Always);
    mpActivateAccessAnnotationsComboBox->addItem(tr("When loading .mol file(s)"), GeneralSettingsPage::Loading);
    mpActivateAccessAnnotationsComboBox->addItem(tr("Never"), GeneralSettingsPage::Never);
    mpActivateAccessAnnotationsComboBox->setCurrentIndex(1);
    setToolTip(mpActivateAccessAnnotationsComboBox, tr("Options for handling of access annotations"), activateAccessAnnotationsDescriptions);
    // mpActivateAccessAnnotationsComboBox->installEventFilter(mpOptionsDialog);
    // create backup file
    mpCreateBackupFileCheckbox = new QCheckBox(tr("Create a model.bak-mo backup file when deleting a model."));
    mpCreateBackupFileCheckbox->setChecked(true);
    /* Display errors/warnings when new instantiation fails in evaluating graphical annotations */
    mpDisplayNFAPIErrorsWarningsCheckBox = new QCheckBox(tr("Display errors/warnings when instantiating the graphical annotations"));
    // set the layout of general settings group
    QGridLayout* pGeneralSettingsLayout = new QGridLayout;
    pGeneralSettingsLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    pGeneralSettingsLayout->addWidget(mpLanguageLabel, 0, 0);
    pGeneralSettingsLayout->addWidget(mpLanguageComboBox, 0, 1, 1, 2);
    pGeneralSettingsLayout->addWidget(mpWorkingDirectoryLabel, 1, 0);
    pGeneralSettingsLayout->addWidget(mpWorkingDirectoryTextBox, 1, 1);
    pGeneralSettingsLayout->addWidget(mpWorkingDirectoryBrowseButton, 1, 2);
    pGeneralSettingsLayout->addWidget(mpToolbarIconSizeLabel, 2, 0);
    pGeneralSettingsLayout->addWidget(mpToolbarIconSizeSpinBox, 2, 1, 1, 2);
    pGeneralSettingsLayout->addWidget(mpPreserveUserCustomizations, 3, 0, 1, 3);
    pGeneralSettingsLayout->addWidget(mpTerminalCommandLabel, 4, 0);
    pGeneralSettingsLayout->addWidget(mpTerminalCommandTextBox, 4, 1);
    pGeneralSettingsLayout->addWidget(mpTerminalCommandBrowseButton, 4, 2);
    pGeneralSettingsLayout->addWidget(mpTerminalCommandArgumentsLabel, 5, 0);
    pGeneralSettingsLayout->addWidget(mpTerminalCommandArgumentsTextBox, 5, 1, 1, 2);
    pGeneralSettingsLayout->addWidget(mpHideVariablesBrowserCheckBox, 6, 0, 1, 3);
    pGeneralSettingsLayout->addWidget(mpActivateAccessAnnotationsLabel, 7, 0);
    pGeneralSettingsLayout->addWidget(mpActivateAccessAnnotationsComboBox, 7, 1, 1, 2);
    pGeneralSettingsLayout->addWidget(mpCreateBackupFileCheckbox, 8, 0, 1, 3);
    pGeneralSettingsLayout->addWidget(mpDisplayNFAPIErrorsWarningsCheckBox, 9, 0, 1, 3);
    mpGeneralSettingsGroupBox->setLayout(pGeneralSettingsLayout);
    // Libraries Browser group box
    mpLibrariesBrowserGroupBox = new QGroupBox(tr("Libraries Browser"));
    // library icon size
    mpLibraryIconSizeLabel = new QLabel(tr("Library Icon Size: *"));
    mpLibraryIconSizeSpinBox = new QSpinBox;
    mpLibraryIconSizeSpinBox->setMinimum(16);
    mpLibraryIconSizeSpinBox->setValue(24);
    // library icon max. text length, value is set later
    mpLibraryIconTextLengthLabel = new QLabel(tr("Max. Library Icon Text Length to Show: *"));
    mpLibraryIconTextLengthSpinBox = new QSpinBox;
    mpLibraryIconTextLengthSpinBox->setMinimum(0);
    mpLibraryIconTextLengthSpinBox->setValue(3);
    // show protected classes
    mpShowProtectedClasses = new QCheckBox(tr("Show Protected Classes"));
    // show hidden classes
    mpShowHiddenClasses = new QCheckBox(tr("Show Hidden Classes (Ignores the annotation(Protection(access = Access.hide)))"));
    // synchronize Libraries Browser with ModelWidget
    mpSynchronizeWithModelWidgetCheckBox = new QCheckBox(tr("Synchronize with Model Widget"));
    mpSynchronizeWithModelWidgetCheckBox->setChecked(true);
    // Libraries Browser group box layout
    QGridLayout* pLibrariesBrowserLayout = new QGridLayout;
    pLibrariesBrowserLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    pLibrariesBrowserLayout->setColumnStretch(1, 1);
    pLibrariesBrowserLayout->addWidget(mpLibraryIconSizeLabel, 0, 0);
    pLibrariesBrowserLayout->addWidget(mpLibraryIconSizeSpinBox, 0, 1);
    pLibrariesBrowserLayout->addWidget(mpLibraryIconTextLengthLabel, 1, 0);
    pLibrariesBrowserLayout->addWidget(mpLibraryIconTextLengthSpinBox, 1, 1);
    pLibrariesBrowserLayout->addWidget(mpShowProtectedClasses, 2, 0, 1, 2);
    pLibrariesBrowserLayout->addWidget(mpShowHiddenClasses, 3, 0, 1, 2);
    pLibrariesBrowserLayout->addWidget(mpSynchronizeWithModelWidgetCheckBox, 4, 0, 1, 2);
    mpLibrariesBrowserGroupBox->setLayout(pLibrariesBrowserLayout);
    // Auto Save
    mpEnableAutoSaveGroupBox = new QGroupBox(tr("Enable Auto Save"));
    mpEnableAutoSaveGroupBox->setToolTip("Auto save feature is experimental. If you encounter unexpected crashes then disable it.");
    mpEnableAutoSaveGroupBox->setCheckable(true);
    mpEnableAutoSaveGroupBox->setChecked(true);
    mpAutoSaveIntervalLabel = new QLabel(tr("Auto Save Interval:"));
    mpAutoSaveIntervalSpinBox = new QSpinBox;
    mpAutoSaveIntervalSpinBox->setSuffix(tr(" seconds"));
    mpAutoSaveIntervalSpinBox->setRange(60, std::numeric_limits<int>::max());
    mpAutoSaveIntervalSpinBox->setSingleStep(30);
    mpAutoSaveIntervalSpinBox->setValue(300);
    mpAutoSaveSecondsLabel = new QLabel;
    connect(mpAutoSaveIntervalSpinBox, SIGNAL(valueChanged(int)), SLOT(autoSaveIntervalValueChanged(int)));
    // Auto Save layout
    QGridLayout* pAutoSaveGridLayout = new QGridLayout;
    pAutoSaveGridLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    pAutoSaveGridLayout->addWidget(mpAutoSaveIntervalLabel, 0, 0);
    pAutoSaveGridLayout->addWidget(mpAutoSaveIntervalSpinBox, 0, 1);
    pAutoSaveGridLayout->addWidget(mpAutoSaveSecondsLabel, 0, 2);
    mpEnableAutoSaveGroupBox->setLayout(pAutoSaveGridLayout);
    // Welcome Page
    mpWelcomePageGroupBox = new QGroupBox(tr("Welcome Page"));
    mpHorizontalViewRadioButton = new QRadioButton(tr("Horizontal View"));
    mpHorizontalViewRadioButton->setChecked(true);
    mpVerticalViewRadioButton = new QRadioButton(tr("Vertical View"));
    QButtonGroup* pWelcomePageViewButtons = new QButtonGroup(this);
    pWelcomePageViewButtons->addButton(mpHorizontalViewRadioButton);
    pWelcomePageViewButtons->addButton(mpVerticalViewRadioButton);
    // plotting view radio buttons layout
    QHBoxLayout* pWelcomePageViewButtonsLayout = new QHBoxLayout;
    pWelcomePageViewButtonsLayout->addWidget(mpHorizontalViewRadioButton);
    pWelcomePageViewButtonsLayout->addWidget(mpVerticalViewRadioButton);

    // Show/hide latest news checkbox
    mpShowLatestNewsCheckBox = new QCheckBox(tr("Show Latest News && Events"));
    mpShowLatestNewsCheckBox->setChecked(true);
    // Recent files and latest news size
    QLabel* pRecentFilesAndLatestNewsSizeLabel = new QLabel(tr("Recent Files and Latest News & Events Size:"));
    mpRecentFilesAndLatestNewsSizeSpinBox = new QSpinBox;
    mpRecentFilesAndLatestNewsSizeSpinBox->setValue(15);
    // Welcome Page layout
    QGridLayout* pWelcomePageGridLayout = new QGridLayout;
    pWelcomePageGridLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    pWelcomePageGridLayout->setColumnStretch(1, 1);
    pWelcomePageGridLayout->addLayout(pWelcomePageViewButtonsLayout, 0, 0, 1, 2, Qt::AlignLeft);
    pWelcomePageGridLayout->addWidget(mpShowLatestNewsCheckBox, 1, 0, 1, 2);
    pWelcomePageGridLayout->addWidget(pRecentFilesAndLatestNewsSizeLabel, 2, 0);
    pWelcomePageGridLayout->addWidget(mpRecentFilesAndLatestNewsSizeSpinBox, 2, 1);
    mpWelcomePageGroupBox->setLayout(pWelcomePageGridLayout);
    // Optional Features Box
    mpOptionalFeaturesGroupBox = new QGroupBox(tr("Optional Features"));
    // Enable instance api
    mpEnableInstanceAPICheckBox = new QCheckBox(tr("Enable instance API *"));
    mpEnableInstanceAPICheckBox->setChecked(true);
    // Optional Features Layout
    QGridLayout* pOptionalFeaturesLayout = new QGridLayout;
    pOptionalFeaturesLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    pOptionalFeaturesLayout->addWidget(mpEnableInstanceAPICheckBox, 0, 0);
    mpOptionalFeaturesGroupBox->setLayout(pOptionalFeaturesLayout);
    // set the layout
    QVBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    pMainLayout->addWidget(mpGeneralSettingsGroupBox);
    pMainLayout->addWidget(mpLibrariesBrowserGroupBox);
    pMainLayout->addWidget(mpEnableAutoSaveGroupBox);
    pMainLayout->addWidget(mpWelcomePageGroupBox);
    pMainLayout->addWidget(mpOptionalFeaturesGroupBox);
    setLayout(pMainLayout);
}

void GeneralSettingsPage::setToolTip(QComboBox* pComboBox, const QString& description, const QStringList& optionsDescriptions)
{
    QString itemsToolTip;
    for (int i = 0; i < pComboBox->count(); ++i)
    {
        // skip empty items
        if (!pComboBox->itemText(i).isEmpty())
        {
            itemsToolTip.append(QString("<li><i>%1</i>").arg(pComboBox->itemText(i)));
            if (optionsDescriptions.size() > i && !optionsDescriptions.at(i).isEmpty())
            {
                itemsToolTip.append(QString(": %1").arg(optionsDescriptions.at(i)));
                pComboBox->setItemData(i, optionsDescriptions.at(i), Qt::ToolTipRole);
            }
            itemsToolTip.append("</li>");
        }
    }
    pComboBox->setToolTip(QString("<html><head/><body><p>%1</p><ul>%2</ul></body></html>").arg(description, itemsToolTip));
}

void GeneralSettingsPage::selectWorkingDirectory()
{
    //语言切换
}

void GeneralSettingsPage::autoSaveIntervalValueChanged(int value)
{
    //自动保存界面
}

LibrariesPage::LibrariesPage(OptionDialog* pOptionsDialog)
    : QWidget(pOptionsDialog)
{
    mpOptionsDialog = pOptionsDialog;
    // MODELICAPATH
    QGroupBox* pModelicaPathGroupBox = new QGroupBox(tr("general"));
    mpModelicaPathLabel = new QLabel("MODELICAPATH");
    mpModelicaPathTextBox = new QLineEdit;
    QString modelicaPathToolTip = tr("List of paths searched while loading a library. Paths are separated by ; on Windows and : on Linux and macOS.");
    mpModelicaPathTextBox->setPlaceholderText(tr("model path"));
    mpModelicaPathTextBox->setToolTip(modelicaPathToolTip);
    mpModelicaPathBrowseButton = new QPushButton(tr("browse"));
    mpModelicaPathBrowseButton->setAutoDefault(false);
    connect(mpModelicaPathBrowseButton, SIGNAL(clicked()), SLOT(selectModelicaPath()));
    // general groupbox layout
    QGridLayout* pGeneralGroupBoxGridLayout = new QGridLayout;
    pGeneralGroupBoxGridLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    pGeneralGroupBoxGridLayout->addWidget(mpModelicaPathLabel, 0, 0);
    pGeneralGroupBoxGridLayout->addWidget(mpModelicaPathTextBox, 0, 1);
    pGeneralGroupBoxGridLayout->addWidget(mpModelicaPathBrowseButton, 0, 2);
    pModelicaPathGroupBox->setLayout(pGeneralGroupBoxGridLayout);
    // system libraries groupbox
    mpSystemLibrariesGroupBox = new QGroupBox(tr("System libraries loaded automatically on startup *"));
    // system libraries note
    mpSystemLibrariesNoteLabel = new QLabel(tr("The system libraries are read from the MODELICAPATH and are always read-only."));
    // load latest Modeica checkbox
    mpLoadLatestModelicaCheckbox = new QCheckBox(tr("Load latest Modelica version on startup"));
    mpLoadLatestModelicaCheckbox->setChecked(true);
    // system libraries tree
    // system libraries tree
    mpSystemLibrariesTree = new QTreeWidget;
    //    mpSystemLibrariesTree->setItemDelegate(new ItemDelegate(mpSystemLibrariesTree));
    mpSystemLibrariesTree->setIndentation(0);
    mpSystemLibrariesTree->setColumnCount(2);
    mpSystemLibrariesTree->setTextElideMode(Qt::ElideMiddle);
    QStringList systemLabels;
    systemLabels << tr("Name") << tr("Version");
    mpSystemLibrariesTree->setHeaderLabels(systemLabels);
    connect(mpSystemLibrariesTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), SLOT(openEditSystemLibrary()));
    // system libraries buttons
    mpAddSystemLibraryButton = new QPushButton(tr("Add"));
    mpAddSystemLibraryButton->setAutoDefault(false);
    connect(mpAddSystemLibraryButton, SIGNAL(clicked()), SLOT(openAddSystemLibrary()));
    mpEditSystemLibraryButton = new QPushButton(tr("Edit"));
    mpEditSystemLibraryButton->setAutoDefault(false);
    connect(mpEditSystemLibraryButton, SIGNAL(clicked()), SLOT(openEditSystemLibrary()));
    mpRemoveSystemLibraryButton = new QPushButton(tr("Remove"));
    mpRemoveSystemLibraryButton->setAutoDefault(false);
    connect(mpRemoveSystemLibraryButton, SIGNAL(clicked()), SLOT(removeSystemLibrary()));
    // system libraries button box
    mpSystemLibrariesButtonBox = new QDialogButtonBox(Qt::Vertical);
    mpSystemLibrariesButtonBox->addButton(mpAddSystemLibraryButton, QDialogButtonBox::ActionRole);
    mpSystemLibrariesButtonBox->addButton(mpEditSystemLibraryButton, QDialogButtonBox::ActionRole);
    mpSystemLibrariesButtonBox->addButton(mpRemoveSystemLibraryButton, QDialogButtonBox::ActionRole);
    // system libraries groupbox layout
    QGridLayout* pSystemLibrariesLayout = new QGridLayout;
    pSystemLibrariesLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    pSystemLibrariesLayout->addWidget(mpSystemLibrariesNoteLabel, 0, 0, 1, 2);
    pSystemLibrariesLayout->addWidget(mpLoadLatestModelicaCheckbox, 1, 0, 1, 2);
    pSystemLibrariesLayout->addWidget(mpSystemLibrariesTree, 2, 0);
    pSystemLibrariesLayout->addWidget(mpSystemLibrariesButtonBox, 2, 1);
    mpSystemLibrariesGroupBox->setLayout(pSystemLibrariesLayout);
    // user libraries groupbox
    mpUserLibrariesGroupBox = new QGroupBox(tr("User libraries loaded automatically on startup *"));
    // user libraries tree
    mpUserLibrariesTree = new QTreeWidget;
    //    mpUserLibrariesTree->setItemDelegate(new ItemDelegate(mpUserLibrariesTree));
    mpUserLibrariesTree->setIndentation(0);
    mpUserLibrariesTree->setColumnCount(2);
    mpUserLibrariesTree->setTextElideMode(Qt::ElideMiddle);
    QStringList userLabels;
    userLabels << tr("Path") << tr("Encoding");
    mpUserLibrariesTree->setHeaderLabels(userLabels);
    connect(mpUserLibrariesTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), SLOT(openEditUserLibrary()));
    // user libraries buttons
    mpAddUserLibraryButton = new QPushButton(tr("Add"));
    mpAddUserLibraryButton->setAutoDefault(false);
    connect(mpAddUserLibraryButton, SIGNAL(clicked()), SLOT(openAddUserLibrary()));
    mpEditUserLibraryButton = new QPushButton(tr("Edit"));
    mpEditUserLibraryButton->setAutoDefault(false);
    connect(mpEditUserLibraryButton, SIGNAL(clicked()), SLOT(openEditUserLibrary()));
    mpRemoveUserLibraryButton = new QPushButton(tr("Remove"));
    mpRemoveUserLibraryButton->setAutoDefault(false);
    connect(mpRemoveUserLibraryButton, SIGNAL(clicked()), SLOT(removeUserLibrary()));
    // user libraries button box
    mpUserLibrariesButtonBox = new QDialogButtonBox(Qt::Vertical);
    mpUserLibrariesButtonBox->addButton(mpAddUserLibraryButton, QDialogButtonBox::ActionRole);
    mpUserLibrariesButtonBox->addButton(mpEditUserLibraryButton, QDialogButtonBox::ActionRole);
    mpUserLibrariesButtonBox->addButton(mpRemoveUserLibraryButton, QDialogButtonBox::ActionRole);
    // user libraries groupbox layout
    QGridLayout* pUserLibrariesLayout = new QGridLayout;
    pUserLibrariesLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    pUserLibrariesLayout->addWidget(mpUserLibrariesTree, 0, 0);
    pUserLibrariesLayout->addWidget(mpUserLibrariesButtonBox, 0, 1);
    mpUserLibrariesGroupBox->setLayout(pUserLibrariesLayout);
    // main layout
    QVBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->addWidget(pModelicaPathGroupBox);
    pMainLayout->addWidget(mpSystemLibrariesGroupBox);
    pMainLayout->addWidget(mpUserLibrariesGroupBox);
    setLayout(pMainLayout);
}

void LibrariesPage::selectModelicaPath()
{
    QMessageBox::information(this, tr("selectModelicaPath"), tr("modelicaPath"), QMessageBox::Ok | QMessageBox::Cancel);
}

void LibrariesPage::openAddSystemLibrary()
{
    AddSystemLibraryDialog* pAddSystemLibraryWidget = new AddSystemLibraryDialog(this);
    pAddSystemLibraryWidget->exec();
}

void LibrariesPage::removeSystemLibrary() {}

void LibrariesPage::openEditSystemLibrary() {}

void LibrariesPage::openAddUserLibrary() {}

void LibrariesPage::removeUserLibrary() {}

void LibrariesPage::openEditUserLibrary() {}

AddSystemLibraryDialog::AddSystemLibraryDialog(LibrariesPage* pLibrariesPage, bool editFlag)
    : QDialog(pLibrariesPage)
    , mpLibrariesPage(pLibrariesPage)
    , mEditFlag(editFlag)
{
    if (mEditFlag)
    {
        setWindowTitle(QString("%1 - %2").arg(tr("CustomSetting"), tr("Edit System Library")));
    }
    else
    {
        setWindowTitle(QString("%1 - %2").arg(tr("CustomSetting"), tr("Add System Library")));
    }
    setAttribute(Qt::WA_DeleteOnClose);
    setMinimumWidth(300);
    mpNameLabel = new QLabel(tr("Name"));
    mpNameComboBox = new QComboBox;
    connect(mpNameComboBox, SIGNAL(currentIndexChanged(QString)), SLOT(getLibraryVersions(QString)));
    mpValueLabel = new QLabel(tr("Version") + ":");
    mpVersionsComboBox = new QComboBox;
    mpVersionsComboBox->setEditable(true);
    mpOkButton = new QPushButton(tr("OK"));
    mpOkButton->setAutoDefault(true);
    connect(mpOkButton, SIGNAL(clicked()), SLOT(addSystemLibrary()));
    mpCancelButton = new QPushButton(tr("OK"));
    mpCancelButton->setAutoDefault(false);
    connect(mpCancelButton, SIGNAL(clicked()), SLOT(reject()));
    // install library button
    mpInstallLibraryButton = new QPushButton(tr("Install library"));
    mpInstallLibraryButton->setAutoDefault(false);
    connect(mpInstallLibraryButton, SIGNAL(clicked()), SLOT(openInstallLibraryDialog()));
    // add buttons
    mpButtonBox = new QDialogButtonBox(Qt::Horizontal);
    mpButtonBox->addButton(mpOkButton, QDialogButtonBox::ActionRole);
    mpButtonBox->addButton(mpCancelButton, QDialogButtonBox::ActionRole);
    // layout
    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(mpNameLabel, 0, 0);
    mainLayout->addWidget(mpNameComboBox, 0, 1);
    mainLayout->addWidget(mpValueLabel, 1, 0);
    mainLayout->addWidget(mpVersionsComboBox, 1, 1);
    mainLayout->addWidget(mpInstallLibraryButton, 2, 0, Qt::AlignLeft);
    mainLayout->addWidget(mpButtonBox, 2, 1, Qt::AlignRight);
    setLayout(mainLayout);
    getSystemLibraries();
}

bool AddSystemLibraryDialog::nameExists(QTreeWidgetItem* pItem)
{
    return true;
}

void AddSystemLibraryDialog::getSystemLibraries() {}

void AddSystemLibraryDialog::getLibraryVersions(const QString& library) {}

void AddSystemLibraryDialog::addSystemLibrary() {}

void AddSystemLibraryDialog::openInstallLibraryDialog() {}
