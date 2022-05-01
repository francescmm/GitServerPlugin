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

      mUrl->setPlaceholderText("GitServer URL");
      mUser->setPlaceholderText("GitServer User");
      mToken->setPlaceholderText("GitServer Token");
   }

private:
   QLineEdit *mUrl;
   QLineEdit *mUser;
   QLineEdit *mToken;

   void saveData()
   {
      QSettings settings;
      settings.setValue("GitServerUrl", mUrl->text());
      settings.setValue("GitServerUser", mUser->text());
      settings.setValue("GitServerToken", mToken->text());

      QDialog::accept();
   }
};

int main(int argc, char *argv[])
{

   qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");

   QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

   QApplication app(argc, argv);
   QSettings settings;

   const auto noInitdata = settings.value("GitServerUrl").toString().isEmpty()
       || settings.value("GitServerUser").toString().isEmpty() || settings.value("GitServerToken").toString().isEmpty();

   if (noInitdata)
   {
      if (const auto dlg = new InputDlg(); dlg->exec() != QDialog::Accepted)
         return 0;
   }

   QMainWindow mainWindow;

   const auto gitServerWidget = new GitServerWidget();

   mainWindow.setCentralWidget(gitServerWidget);

   GitServerPlugin::ConfigData config { settings.value("GitServerUrl").toString(),
                                        settings.value("GitServerUser").toString(),
                                        settings.value("GitServerToken").toString(),
                                        {},
                                        {},
                                        {} };

   gitServerWidget->configure(std::move(config), {}, "");
   mainWindow.show();

   return app.exec();
}
