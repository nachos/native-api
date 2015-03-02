'use strict';

var nativeApi = require('../../lib/native-api');
var process = nativeApi.process;
var chai = require('chai');
var expect = chai.expect;
var _ = require('lodash');

describe('process', function () {

  describe('getAllProcesses', function() {

  	var processNames;
  	before(function() {
		  processNames = _.map(process.getAllProcesses(), 'name');
    });

  	it('return node as a running process', function () {
      expect(processNames).not.to.be.empty;
      expect(processNames).to.have.length.above(1);
      expect(processNames).to.include('node.exe');
    });

  })

});
