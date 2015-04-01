'use strict';

var window = require('./');
var chai = require('chai');
var expect = chai.expect;
var _ = require('lodash');

describe('window', function () {

  describe('getAllWindows', function() {

    it('return at least one window', function () {
      var windows = window.getAllWindows();
      expect(windows).to.have.length.above(0);
    });
  });
});