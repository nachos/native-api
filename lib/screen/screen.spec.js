'use strict';

var screen = require('./');
var chai = require('chai');
var expect = chai.expect;
var _ = require('lodash');

describe('screen', function () {

  describe('getAllScreens', function() {

    it('return at least one screen', function () {
      var screens = screen.getAllScreens();
      expect(screens).to.have.length.above(0);
    });

    it('exactly one primary screen', function () {
      var screens = screen.getAllScreens();
      var primaryScreens = _.where(screens, { primary: true });
      expect(primaryScreens.length).to.equal(1);
    });
  })
});