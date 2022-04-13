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

#include <QFrame>
#include <QVector>

class GitBase;
class FileDiffView;
class PrChangeListItem;

namespace GitServerPlugin
{
struct PullRequest;
}

class PrChangesList : public QFrame
{
   Q_OBJECT

signals:
   void gotoReview(int linkId);
   void addCodeReview(int line, const QString &path, const QString &body);

public:
   explicit PrChangesList(const QSharedPointer<GitBase> &git, const QString &style, QWidget *parent = nullptr);

   void loadData(const GitServerPlugin::PullRequest &prInfo);

   void onReviewsReceived(GitServerPlugin::PullRequest pr);
   void addLinks(GitServerPlugin::PullRequest pr, const QMap<int, int> &reviewLinkToComments);

private:
   QSharedPointer<GitBase> mGit;
   QString mStyle;
   FileDiffView *mOldFile = nullptr;
   FileDiffView *mNewFile = nullptr;
   QVector<PrChangeListItem *> mListItems;
};
