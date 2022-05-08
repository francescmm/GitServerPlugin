#include <GitBase.h>
#include <QApplication>
#include <QDialog>
#include <QGridLayout>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QSettings>

#include <GitServerWidget.h>

class InputDlg : public QDialog
{
public:
   InputDlg(QWidget *parent = nullptr)
      : QDialog(parent)
   {
      auto layout = new QGridLayout(this);
      layout->addWidget(mUrl = new QLineEdit(), 0, 0);
      layout->addWidget(mUser = new QLineEdit(), 1, 0);
      layout->addWidget(mToken = new QLineEdit(), 2, 0);

      auto pbAccept = new QPushButton(tr("Save"));
      connect(pbAccept, &QPushButton::clicked, this, &InputDlg::saveData);

      layout->addWidget(pbAccept, 3, 0);

      setAttribute(Qt::WA_DeleteOnClose);

      mUrl->setPlaceholderText("GitServer Repo URL");
      mUser->setPlaceholderText("GitServer Repo owner");
      mToken->setPlaceholderText("GitServer Repo name");
   }

private:
   QLineEdit *mUrl;
   QLineEdit *mUser;
   QLineEdit *mToken;

   void saveData()
   {
      QSettings settings;
      settings.setValue("GitRepoUrl", mUrl->text());
      settings.setValue("GitRepoOwner", mUser->text());
      settings.setValue("GitRepoName", mToken->text());

      QDialog::accept();
   }
};

int main(int argc, char *argv[])
{

   qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");

   QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

   QApplication app(argc, argv);
   QSettings settings;
   GitServerPlugin::ConfigData config;

   if (argc <= 1)
   {
      if (const auto dlg = new InputDlg(); dlg->exec() == QDialog::Accepted)
      {
         config = decltype(
             config) { settings.value("GitRepoOwner").toString(), {},
                       settings.value("GitRepoUrl").toString(),   {},
                       settings.value("GitRepoName").toString(),  settings.value("GitRepoOwner").toString() };
      }
      else
         return 0;
   }
   else
      config = decltype(config) { argv[2], {}, argv[1], {}, argv[3], argv[2] };

   QMainWindow mainWindow;

   QSharedPointer<GitBase> git(new GitBase(QString::fromUtf8(SOURCE_PATH)));

   const auto gitServerWidget = new GitServerWidget(git, &mainWindow);

   mainWindow.setCentralWidget(gitServerWidget);
   mainWindow.show();

   if (gitServerWidget->configure(std::move(config), {}, ""))
      return app.exec();

   return 0;
}
