'use strict';

var path = require('./');
var chai = require('chai');
var expect = chai.expect;
var fs = require('fs');

describe('path', function () {

  describe('getUserHome', function() {

    it('Path exists', function (done) {
      var userHome = path.getUserHome();
      fs.exists(userHome, function (exists) {
        expect(exists).to.be.true;
        done();
      })
    });
  });
});