'use strict';

var process = require('./');
var chai = require('chai');
var expect = chai.expect;
var _ = require('lodash');

describe('process', function () {

  describe('getAllProcesses', function() {

    it('return node as a running process', function () {
      var processNames = _.pluck(process.getAllProcesses(), 'name');
      expect(processNames).not.to.be.empty;
      expect(processNames).to.have.length.above(1);
      expect(processNames).to.include('node.exe');
    });
  });
});