/*
 * native-api
 * https://github.com/nachos/native-api
 *
 * Copyright (c) 2015
 * Licensed under the MIT license.
 */

'use strict';

var bindings = require('bindings');
var process = bindings('process');
var file = bindings('file');

module.exports = {
  process: process,
  file: file
};
