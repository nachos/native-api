'use strict';

var process = require('./process');
var file = require('./file');
var screen = require('./screen');
var path = require('./path');
var window = require('./window');
var fileAssociation = require('./file-association');

module.exports = {
  process: process,
  file: file,
  screen: screen,
  window: window,
  path: path,
  fileAssociation: fileAssociation
};