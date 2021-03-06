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

#include "core/redis/config.h"

#include <string>
#include <algorithm>
#include <vector>

#include "common/utils.h"

#include "fasto/qt/logger.h"

#define DEFAULT_REDIS_SERVER_PORT 6379

namespace fastonosql {
namespace core {
namespace redis {
namespace {

void parseOptions(int argc, char** argv, Config& cfg) {
  for (int i = 0; i < argc; i++) {
    const bool lastarg = i == argc - 1;

    if (!strcmp(argv[i], "-h") && !lastarg) {
      cfg.host.host = argv[++i];
    }/* else if (!strcmp(argv[i], "-h") && lastarg) {
      usage();
    } else if (!strcmp(argv[i], "--help")) {
      usage();
    } else if (!strcmp(argv[i], "-x")) {
      cfg.stdinarg = 1;
    }*/ else if (!strcmp(argv[i], "-p") && !lastarg) {
      cfg.host.port = common::ConvertFromString<uint16_t>(argv[++i]);
    } else if (!strcmp(argv[i], "-s") && !lastarg) {
      cfg.hostsocket = argv[++i];
    } else if (!strcmp(argv[i], "-r") && !lastarg) {
      cfg.repeat = strtoll(argv[++i], NULL, 10);
    } else if (!strcmp(argv[i], "-i") && !lastarg) {
      double seconds = atof(argv[++i]);
      cfg.interval = seconds*1000000;
    } else if (!strcmp(argv[i], "-n") && !lastarg) {
      cfg.dbnum = common::ConvertFromString<int>(argv[++i]);
    } else if (!strcmp(argv[i], "-a") && !lastarg) {
      cfg.auth = argv[++i];
    }
    /*else if (!strcmp(argv[i], "--raw")) {
      cfg.output = OUTPUT_RAW;
    } else if (!strcmp(argv[i], "--no-raw")) {
      cfg.output = OUTPUT_STANDARD;
    } else if (!strcmp(argv[i], "--csv")) {
      cfg.output = OUTPUT_CSV;
    }*/ else if (!strcmp(argv[i], "--latency")) {
      cfg.latency_mode = 1;
    } else if (!strcmp(argv[i], "--latency-history")) {
      cfg.latency_mode = 1;
      cfg.latency_history = 1;
    } else if (!strcmp(argv[i], "--slave")) {
      cfg.slave_mode = 1;
    } else if (!strcmp(argv[i], "--stat")) {
      cfg.stat_mode = 1;
    } else if (!strcmp(argv[i], "--scan")) {
      cfg.scan_mode = 1;
    } else if (!strcmp(argv[i], "--pattern") && !lastarg) {
      cfg.pattern = argv[++i];
    } else if (!strcmp(argv[i], "--intrinsic-latency") && !lastarg) {
      cfg.intrinsic_latency_mode = 1;
      cfg.intrinsic_latency_duration = common::ConvertFromString<int>(argv[++i]);
    } else if (!strcmp(argv[i], "--rdb") && !lastarg) {
      cfg.getrdb_mode = 1;
      cfg.rdb_filename = argv[++i];
    /*} else if (!strcmp(argv[i], "--pipe")) {
      cfg.pipe_mode = 1;
    } else if (!strcmp(argv[i], "--pipe-timeout") && !lastarg) {
      cfg.pipe_timeout = common::ConvertFromString<int>(argv[++i]);*/
    } else if (!strcmp(argv[i], "--bigkeys")) {
      cfg.bigkeys = 1;
    } else if (!strcmp(argv[i], "--eval") && !lastarg) {
      cfg.eval = argv[++i];
    } else if (!strcmp(argv[i], "-c")) {
      cfg.cluster_mode = 1;
    } else if (!strcmp(argv[i], "-d") && !lastarg) {
      cfg.delimiter = argv[++i];
    } else if (!strcmp(argv[i], "-ns") && !lastarg) {
      cfg.ns_separator = argv[++i];
    }
    /*else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
      sds version = cliVersion();
      printf("redis-cli %s\n", version);
      sdsfree(version);
    }*/
    else {
      if (argv[i][0] == '-') {
        const uint16_t size_buff = 256;
        char buff[size_buff] = {0};
        sprintf(buff, "Unrecognized option or bad number of args for: '%s'", argv[i]);
        LOG_MSG(buff, common::logging::L_WARNING, true);
        break;
      } else {
        /* Likely the command name, stop here. */
        break;
      }
    }
  }
}

}  // namespace

Config::Config()
  : RemoteConfig(common::net::hostAndPort::createLocalHost(DEFAULT_REDIS_SERVER_PORT)) {
  init();
}

Config::Config(const Config& other)
  : RemoteConfig(other.host) {
  init();
  copy(other);
}

Config& Config::operator=(const Config& other) {
  copy(other);
  return *this;
}

void Config::copy(const Config& other) {
  hostsocket = other.hostsocket;

  repeat = other.repeat;
  interval = other.interval;
  dbnum = other.dbnum;
  interactive = other.interactive;
  monitor_mode = other.monitor_mode;
  pubsub_mode = other.pubsub_mode;
  latency_mode = other.latency_mode;
  latency_history = other.latency_history;
  cluster_mode = other.cluster_mode;
  cluster_reissue_command = other.cluster_reissue_command;
  slave_mode = other.slave_mode;
  getrdb_mode = other.getrdb_mode;
  stat_mode = other.stat_mode;
  scan_mode = other.scan_mode;
  intrinsic_latency_mode = other.intrinsic_latency_mode;
  intrinsic_latency_duration = other.intrinsic_latency_duration;

  pattern = other.pattern;
  rdb_filename = other.rdb_filename;

  bigkeys = other.bigkeys;

  auth = other.auth;
  eval = other.eval;

  last_cmd_type = other.last_cmd_type;

  RemoteConfig::operator=(other);
}

void Config::init() {
  hostsocket = std::string();
  repeat = 1;
  interval = 0;
  dbnum = 0;
  interactive = 0;
  monitor_mode = 0;
  pubsub_mode = 0;
  latency_mode = 0;
  latency_history = 0;
  cluster_mode = 0;
  slave_mode = 0;
  getrdb_mode = 0;
  stat_mode = 0;
  scan_mode = 0;
  intrinsic_latency_mode = 0;
  intrinsic_latency_duration = 0;
  cluster_reissue_command = 0;
  pattern = std::string();
  rdb_filename = std::string();
  bigkeys = 0;
  auth = std::string();
  eval = std::string();
  last_cmd_type = -1;
}

}  // namespace redis
}  // namespace core
}  // namespace fastonosql

namespace common {

std::string ConvertToString(const fastonosql::core::redis::Config& conf) {
  std::vector<std::string> argv = conf.args();

  if (!conf.hostsocket.empty()) {
    argv.push_back("-s");
    argv.push_back(conf.hostsocket);
  }
  if (conf.repeat) {
    argv.push_back("-r");
    argv.push_back(ConvertToString(conf.repeat));
  }
  if (conf.interval) {
    argv.push_back("-i");
    argv.push_back(ConvertToString(conf.interval/1000000));
  }
  if (conf.dbnum) {
    argv.push_back("-n");
    argv.push_back(ConvertToString(conf.dbnum));
  }

  if (!conf.auth.empty()) {
    argv.push_back("-a");
    argv.push_back(conf.auth);
  }

  if (conf.latency_mode) {
    if (conf.latency_history) {
      argv.push_back("--latency-history");
    } else {
      argv.push_back("--latency");
    }
  }

  if (conf.slave_mode) {
    argv.push_back("--slave");
  }
  if (conf.stat_mode) {
    argv.push_back("--stat");
  }
  if (conf.scan_mode) {
    argv.push_back("--scan");
  }
  if (!conf.pattern.empty()) {
    argv.push_back("--pattern");
    argv.push_back(conf.pattern);
  }
  if (conf.intrinsic_latency_mode) {
    argv.push_back("--intrinsic-latency");
    argv.push_back(ConvertToString(conf.intrinsic_latency_mode));
    argv.push_back(ConvertToString(conf.intrinsic_latency_duration));
  }

  if (conf.getrdb_mode) {
    argv.push_back("--rdb");
    argv.push_back(conf.rdb_filename);
  }
  if (conf.bigkeys) {
    argv.push_back("--bigkeys");
  }

  if (!conf.eval.empty()) {
    argv.push_back("--eval");
    argv.push_back(conf.eval);
  }

  if (conf.cluster_mode) {
    argv.push_back("-c");
  }

  std::string result;
  for (size_t i = 0; i < argv.size(); ++i) {
    result+= argv[i];
    if (i != argv.size() - 1) {
      result+=" ";
    }
  }

  return result;
}

template<>
fastonosql::core::redis::Config ConvertFromString(const std::string& line) {
  fastonosql::core::redis::Config cfg;
  enum { kMaxArgs = 64 };
  int argc = 0;
  char* argv[kMaxArgs] = {0};

  char* p2 = strtok((char*)line.c_str(), " ");
  while (p2) {
    argv[argc++] = p2;
    p2 = strtok(0, " ");
  }

  fastonosql::core::redis::parseOptions(argc, argv, cfg);
  return cfg;
}

}  // namespace common
