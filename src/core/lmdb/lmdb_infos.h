/*  Copyright (C) 2014-2016 FastoGT. All right reserved.

    This file is part of FastoNoSQL.

    SiteOnYourDevice is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SiteOnYourDevice is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SiteOnYourDevice.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <string>

#include "core/types.h"

#define LMDB_STATS_LABEL "# Stats"

#define LMDB_CAMPACTIONS_LEVEL_LABEL "compactions_level"
#define LMDB_FILE_SIZE_MB_LABEL "file_size_mb"
#define LMDB_TIME_SEC_LABEL "time_sec"
#define LMDB_READ_MB_LABEL "read_mb"
#define LMDB_WRITE_MB_LABEL "write_mb"

namespace fastonosql {

class LmdbServerInfo
  : public ServerInfo {
 public:
  // Compactions\nLevel  Files Size(MB) Time(sec) Read(MB) Write(MB)\n
  struct Stats
          : FieldByIndex {
      Stats();
      explicit Stats(const std::string& common_text);
      common::Value* valueByIndex(unsigned char index) const;

      uint32_t compactions_level_;
      uint32_t file_size_mb_;
      uint32_t time_sec_;
      uint32_t read_mb_;
      uint32_t write_mb_;
  } stats_;

  LmdbServerInfo();
  explicit LmdbServerInfo(const Stats& stats);
  virtual common::Value* valueByIndexes(unsigned char property, unsigned char field) const;
  virtual std::string toString() const;
  virtual uint32_t version() const;
};

std::ostream& operator << (std::ostream& out, const LmdbServerInfo& value);

LmdbServerInfo* makeLmdbServerInfo(const std::string &content);
LmdbServerInfo* makeLmdbServerInfo(FastoObject *root);

class LmdbDataBaseInfo
      : public DataBaseInfo {
 public:
  LmdbDataBaseInfo(const std::string& name, bool isDefault, size_t size,
                   const keys_cont_type& keys = keys_cont_type());
  virtual DataBaseInfo* clone() const;
};

class LmdbCommand
      : public FastoObjectCommand {
 public:
  LmdbCommand(FastoObject* parent, common::CommandValue* cmd, const std::string &delemitr);
  virtual bool isReadOnly() const;
};

}  // namespace fastonosql
