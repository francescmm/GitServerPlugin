#pragma once

/****************************************************************************************
 ** GitQlient is an application to manage and operate one or several Git repositories. With
 ** GitQlient you will be able to add commits, branches and manage all the options Git provides.
 ** Copyright (C) 2021  Francesc Martinez
 **
 ** LinkedIn: www.linkedin.com/in/cescmm/
 ** Web: www.francescmm.com
 **
 ** This program is free software; you can redistribute it and/or
 ** modify it under the terms of the GNU Lesser General Public
 ** License as published by the Free Software Foundation; either
 ** version 2 of the License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 ** Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public
 ** License along with this library; if not, write to the Free Software
 ** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 ***************************************************************************************/

#include <IGitServerWidget.h>

#include <QFrame>

class IssueDetailedView;
class QPushButton;
class QStackedLayout;
class CreateIssueDlg;
class CreatePullRequestDlg;
class GitServerCache;

namespace GitServerPlugin
{
class IRestApi;
struct Issue;
}

class GitServerWidget final : public IGitServerWidget
{
   Q_OBJECT
   Q_PLUGIN_METADATA(IID "francescmm.GitServerPlugin" FILE "GitServerPlugin.json")
   Q_INTERFACES(IGitServerWidget)

public:
   explicit GitServerWidget(QWidget *parent = nullptr);
   explicit GitServerWidget(const QSharedPointer<GitBase> &git, QWidget *parent = nullptr);

   virtual ~GitServerWidget() override;

   bool configure(const GitServerPlugin::ConfigData &config, const QVector<QPair<QString, QStringList>> &remoteBranches,
                  const QString &styles) override;

   bool isConfigured() const override { return mConfigured; }

   void openPullRequest(int prNumber) override;

   void start(const QVector<QPair<QString, QStringList>> &remoteBranches) override;

   QSharedPointer<IGitServerCache> getCache() override;

   IGitServerWidget *createWidget(const QSharedPointer<GitBase> &git) override;

private:
   QSharedPointer<GitBase> mGit;
   QSharedPointer<GitServerCache> mGitServerCache;
   QStackedLayout *mStackedLayout = nullptr;
   IssueDetailedView *mDetailedView = nullptr;
   QFrame *mGeneralView = nullptr;
   CreateIssueDlg *mCreateIssueView = nullptr;
   CreatePullRequestDlg *mCreatePrView = nullptr;
   QPushButton *mOldIssue = nullptr;
   QPushButton *mOldPr = nullptr;
   QPushButton *mRefresh = nullptr;
   QVector<QPair<QString, QStringList>> mRemoteBranches;
};
