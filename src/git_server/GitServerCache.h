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

#include <QMap>
#include <QObject>
#include <QVector>

#include <GitServerTypes.h>
#include <IGitServerCache.h>

namespace GitServerPlugin
{
struct Commit;
}

class GitServerCache : public QObject, public IGitServerCache
{
   Q_OBJECT

signals:
   void connectionTested();
   void issueUpdated(const GitServerPlugin::Issue &issue);
   void issuesReceived();
   void prUpdated(GitServerPlugin::PullRequest pr);
   void prReceived();
   void prReviewsReceived();
   void errorOccurred(const QString &error);

public:
   explicit GitServerCache(QObject *parent = nullptr);
   ~GitServerCache() override;

   bool init(GitServerPlugin::ConfigData data) override;

   QString getUserName() const override;

   QVector<GitServerPlugin::PullRequest> getPullRequests() const override;
   GitServerPlugin::PullRequest getPullRequest(int number) const override;
   GitServerPlugin::PullRequest getPullRequest(const QString &sha) const override;
   QVector<GitServerPlugin::Issue> getIssues() const override;
   GitServerPlugin::Issue getIssue(int number) const override { return mIssues.value(number); }
   QVector<GitServerPlugin::Label> getLabels() const override { return mLabels; }
   QVector<GitServerPlugin::Milestone> getMilestones() const override { return mMilestones; }

   GitServerPlugin::Platform getPlatform() const override;
   GitServerPlugin::IRestApi *getApi() const override;

private:
   bool mInit = false;
   int mPreSteps = -1;
   bool mWaitingConfirmation = false;
   QScopedPointer<GitServerPlugin::IRestApi> mApi;
   QMap<int, GitServerPlugin::PullRequest> mPullRequests;
   QMap<int, GitServerPlugin::Issue> mIssues;
   QVector<GitServerPlugin::Label> mLabels;
   QVector<GitServerPlugin::Milestone> mMilestones;

   void triggerSignalConditionally();

   void onConnectionTested();
   void onIssueUpdated(const GitServerPlugin::Issue &issue);
   void onPRUpdated(const GitServerPlugin::PullRequest &pr);
   void onCommentsReceived(int number, const QVector<GitServerPlugin::Comment> &comments);
   void onCodeReviewsReceived(int number, const QVector<GitServerPlugin::CodeReview> &codeReviews);
   void onCommentReviewsReceived(int number, const QMap<int, GitServerPlugin::Review> &commentReviews);
   void onCommitsReceived(int number, const QVector<GitServerPlugin::Commit> &commits, int currentPage, int lastPage);

   void initLabels(const QVector<GitServerPlugin::Label> &labels);
   void initMilestones(const QVector<GitServerPlugin::Milestone> &milestones);
   void initIssues(const QVector<GitServerPlugin::Issue> &issues);
   void initPullRequests(const QVector<GitServerPlugin::PullRequest> &prs);
};
