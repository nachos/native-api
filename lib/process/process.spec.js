'use strict';

var process = require('./');
var chai = require('chai');
var expect = chai.expect;
var _ = require('lodash');

describe('process', function () {

  describe('getAllProcesses', function() {

    it('return node as a running process', function () {
      var data = process.getAllProcesses();
      expect(data).to.have.property('errorCode');
      expect(data.errorCode).to.equal(0);
      expect(data).to.have.property('processes');
      expect(data.processes).not.to.be.empty;
      expect(data.processes).to.have.length.above(1);
      var processNames = _.pluck(data.processes, 'name');
      expect(processNames).to.include('node.exe');
    });
  });
});