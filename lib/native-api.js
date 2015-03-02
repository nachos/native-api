/*
 * native-api
 * https://github.com/nachos/native-api
 *
 * Copyright (c) 2015
 * Licensed under the MIT license.
 */

'use strict';

var process = require('bindings')('process');

module.exports = {
  process: process
};
