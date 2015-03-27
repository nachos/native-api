'use strict';

var getUserHome = function () {
  return process.env[(process.platform === 'win32') ? 'USERPROFILE' : 'HOME'];
};

module.exports = {
  getUserHome: getUserHome
};