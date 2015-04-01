'use strict';

var icon = require('./');
var chai = require('chai');
var expect = chai.expect;
var _ = require('lodash');

describe('icon', function () {

  describe('getIcon', function() {

    it('return at least one window', function () {
      var ico = icon.getIcon('C:\\Program Files\\nodejs\\node.exe');
      //console.log(ico);
      //expect(icon).to.have.length.above(0);
    });
  });
});