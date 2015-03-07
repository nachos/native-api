'use strict';

var nativeApi = require('../../../lib/native-api');
var file = nativeApi.file;
var chai = require('chai');
var expect = chai.expect;
var _ = require('lodash');

describe('file', function () {

  describe('getFileStats', function() {

    before(function() {
      // Set file attributes for the tests because git does not save file attributes
      var exec = require('child_process').exec;
      exec('attrib +S systemFile & attrib +R readOnly & attrib +H hidden & attrib -S -R -H normal', function (error) {
        expect(error).to.not.exist;
      });
    });

    it('returns correct stats for a normal file', function () {
      var fileStats = file.getFileStats('./normal');

      expect(fileStats).to.exist;
      expect(fileStats).to.not.have.property('error');
      expect(fileStats).to.have.property('isNormal', true);
    });

    it('returns correct stats for a hidden file', function () {
      var fileStats = file.getFileStats('./hidden');

      expect(fileStats).to.exist;
      expect(fileStats).to.not.have.property('error');
      expect(fileStats).to.have.property('isHidden', true);
    });

    it('returns correct stats for a read only file', function () {
      var fileStats = file.getFileStats('./readOnly');

      expect(fileStats).to.exist;
      expect(fileStats).to.not.have.property('error');
      expect(fileStats).to.have.property('isReadOnly', true);
    });

    it('returns correct stats for a directory', function () {
      var fileStats = file.getFileStats('./');

      expect(fileStats).to.exist;
      expect(fileStats).to.not.have.property('error');
      expect(fileStats).to.have.property('isDirectory', true);
    });

    it('returns correct stats for a system file', function () {
      var fileStats = file.getFileStats("./systemFile");

      expect(fileStats).to.exist;
      expect(fileStats).to.not.have.property('error');
      expect(fileStats).to.have.property('isSystem', true);
    });

    it('returns an error for non existant file', function () {
      var fileStats = file.getFileStats('./nonExistant.txt');

      expect(fileStats).to.exist;
      expect(fileStats).to.have.property('error', 'file not found');
    });

  })

});
