'use strict';

var bindings = require('bindings');
var path = require('path');
var file = bindings('file');

module.exports = {
  getFileStats: function(filePath) {
    return file.getFileStats(path.resolve(filePath));
  }
};