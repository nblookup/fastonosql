/*  Copyright (C) 2014-2016 FastoGT. All right reserved.

    This file is part of FastoNoSQL.

    FastoNoSQL is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FastoNoSQL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FastoNoSQL.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma  once

#include <QDialog>

#include "core/core_fwd.h"
#include "core/events/events_info.h"

class QLineEdit;

namespace fasto {
namespace qt {
namespace gui {
class GlassWidget;
}
}
}

namespace fastonosql {
namespace gui {

class ChangePasswordServerDialog
  : public QDialog {
  Q_OBJECT
 public:
  enum {
    fix_height = 120,
    fix_width = 240
  };

  explicit ChangePasswordServerDialog(const QString& title, core::IServerSPtr server,
                                      QWidget* parent = 0);

 private Q_SLOTS:
  void tryToCreatePassword();
  void startChangePassword(const core::events_info::ChangePasswordRequest& req);
  void finishChangePassword(const core::events_info::ChangePasswordResponce& res);

 private:
  bool validateInput();

  fasto::qt::gui::GlassWidget* glassWidget_;
  QLineEdit* passwordLineEdit_;
  QLineEdit* confPasswordLineEdit_;
  const core::IServerSPtr server_;
};

}  // namespace gui
}  // namespace fastonosql
