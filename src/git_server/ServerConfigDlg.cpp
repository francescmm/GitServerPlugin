#include "ServerConfigDlg.h"
#include "ui_ServerConfigDlg.h"

#include <GitHubRestApi.h>
#include <GitLabRestApi.h>
#include <GitServerCache.h>

#include <QLogger.h>

#include <QDesktopServices>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>

#include <map>
#include <utility>

using namespace QLogger;
using namespace GitServerPlugin;

namespace
{
enum class GitServerPlatform
{
   GitHub,
   GitHubEnterprise,
   GitLab,
   Bitbucket
};

static const std::map<GitServerPlatform, const char *> repoUrls {
   { GitServerPlatform::GitHub, "https://api.github.com" },
   { GitServerPlatform::GitHubEnterprise, "" },
   { GitServerPlatform::GitLab, "https://gitlab.com/api/v4" }
};
}

ServerConfigDlg::ServerConfigDlg(const QSharedPointer<GitServerCache> &gitServerCache,
                                 const GitServerPlugin::ConfigData &data, const QString &styleSheet, QWidget *parent)
   : QDialog(parent)
   , ui(new Ui::ServerConfigDlg)
   , mGitServerCache(gitServerCache)
   , mData(data)
   , mManager(new QNetworkAccessManager())
{
   setStyleSheet(styleSheet);

   ui->setupUi(this);

   connect(ui->cbServer, &QComboBox::currentTextChanged, this, &ServerConfigDlg::onServerChanged);

   ui->leEndPoint->setHidden(true);

   ui->leUserName->setText(mData.user);
   ui->leUserToken->setText(mData.token);
   ui->leEndPoint->setText(mData.endPoint);

   ui->cbServer->insertItem(static_cast<int>(GitServerPlatform::GitHub), "GitHub",
                            repoUrls.at(GitServerPlatform::GitHub));
   ui->cbServer->insertItem(static_cast<int>(GitServerPlatform::GitHubEnterprise), "GitHub Enterprise",
                            repoUrls.at(GitServerPlatform::GitHubEnterprise));

   if (mData.serverUrl.contains("github"))
      ui->cbServer->setCurrentIndex(static_cast<int>(GitServerPlatform::GitHub));
   else
   {
      ui->cbServer->insertItem(static_cast<int>(GitServerPlatform::GitLab), "GitLab",
                               repoUrls.at(GitServerPlatform::GitLab));
      ui->cbServer->setCurrentIndex(static_cast<int>(GitServerPlatform::GitLab));
      ui->cbServer->setVisible(false);
   }

   ui->lAccessToken->setText(tr("How to get a token?"));
   connect(ui->lAccessToken, &ButtonLink::clicked, [isGitHub = mData.serverUrl.contains("github")]() {
      const auto url = isGitHub
          ? "https://docs.github.com/en/github/authenticating-to-github/creating-a-personal-access-token"
          : "https://docs.gitlab.com/ee/user/profile/personal_access_tokens.html";
      QDesktopServices::openUrl(QUrl(QString::fromUtf8(url)));
   });

   connect(ui->leUserToken, &QLineEdit::editingFinished, this, &ServerConfigDlg::checkToken);
   connect(ui->leUserToken, &QLineEdit::returnPressed, this, &ServerConfigDlg::accept);
   connect(ui->pbAccept, &QPushButton::clicked, this, &ServerConfigDlg::accept);
   connect(ui->pbCancel, &QPushButton::clicked, this, &ServerConfigDlg::reject);
   connect(ui->pbTest, &QPushButton::clicked, this, &ServerConfigDlg::testToken);
}

ServerConfigDlg::~ServerConfigDlg()
{
   delete mManager;
   delete ui;
}

ConfigData ServerConfigDlg::getNewConfigData() const
{
   return GitServerPlugin::ConfigData { ui->leUserName->text(), ui->leUserToken->text(), mData.serverUrl, mEndPoint,
                                        mData.repoName,         mData.repoOwner };
}

void ServerConfigDlg::checkToken()
{
   if (ui->leUserToken->text().isEmpty())
      ui->leUserName->setStyleSheet("border: 1px solid red;");
}

void ServerConfigDlg::accept()
{
   mEndPoint = ui->cbServer->currentIndex() == static_cast<int>(GitServerPlatform::GitHubEnterprise)
       ? ui->leEndPoint->text()
       : ui->cbServer->currentData().toString();

   QDialog::accept();
}

void ServerConfigDlg::testToken()
{
   if (ui->leUserToken->text().isEmpty())
      ui->leUserName->setStyleSheet("border: 1px solid red;");
   else
   {
      const auto endpoint = ui->cbServer->currentIndex() == static_cast<int>(GitServerPlatform::GitHubEnterprise)
          ? ui->leEndPoint->text()
          : ui->cbServer->currentData().toString();
      IRestApi *api = nullptr;

      if (ui->cbServer->currentIndex() == static_cast<int>(GitServerPlatform::GitLab))
      {
         api = new GitLabRestApi(ui->leUserName->text(), mData.repoName, mData.serverUrl,
                                 { ui->leUserName->text(), ui->leUserToken->text(), endpoint }, this);
      }
      else
      {
         api = new GitHubRestApi(mData.repoOwner, mData.repoName,
                                 { ui->leUserName->text(), ui->leUserToken->text(), endpoint }, this);
      }

      api->testConnection();

      connect(api, &IRestApi::connectionTested, this, &ServerConfigDlg::onTestSucceeded);
      connect(api, &IRestApi::errorOccurred, this,
              [this](const QString &error) { QMessageBox::warning(this, tr("API access error!"), error); });
   }
}

void ServerConfigDlg::onServerChanged()
{
   ui->leEndPoint->setVisible(ui->cbServer->currentIndex() == static_cast<int>(GitServerPlatform::GitHubEnterprise));
}

void ServerConfigDlg::onTestSucceeded()
{
   ui->lTestResult->setText(tr("Token confirmed!"));
   QTimer::singleShot(3000, ui->lTestResult, &QLabel::clear);
}
