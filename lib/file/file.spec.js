'use strict';

var fs = require('fs');
var path = require('path');
var file = require('./');
var chai = require('chai');
var expect = chai.expect;
var async = require('async');
var exec = require('child_process').exec;
var mkdirp = require('mkdirp');
var del = require('del');

describe('file', function () {

  describe('getFileStats', function() {

    before(function(done) {
      var files = [
        {
          name: '.hidden',
          winAttribCommand: 'attrib +H'
        },
        {
          name: 'readOnly',
          winAttribCommand: 'attrib +R'
        },
        {
          name: 'normal',
          winAttribCommand: 'attrib -S -R -H -A'
        },
        {
          name: 'systemFile',
          winAttribCommand: 'attrib +S'
        },
        {
          name: 'archive',
          winAttribCommand: 'attrib +A'
        }
      ];

      mkdirp('.tmp', function(error){
        if(error){
          return done(error);
        }

        async.each(files, function(file, cb){
          fs.writeFile(path.join('.tmp', file.name), 'bla', function(err){
            if(err){
              return cb(err);
            }

            exec(file.winAttribCommand + ' ' + file.name, {cwd:'.tmp'}, cb);
          });
        }, done);
      });
    });

    after(function(done) {
      del('.tmp', done);
    });

    it('returns correct stats for a normal file', function () {
      var fileStats = file.getFileStats('./.tmp/normal');

      expect(fileStats).to.exist;
      expect(fileStats.errorCode).to.equal(0);
      expect(fileStats).to.have.property('isNormal', true);
    });

    it('returns correct stats for a archive file', function () {
      var fileStats = file.getFileStats('./.tmp/archive');

      expect(fileStats).to.exist;
      expect(fileStats.errorCode).to.equal(0);
      expect(fileStats).to.have.property('isArchive', true);
    });

    it('returns correct stats for a hidden file', function () {
      var fileStats = file.getFileStats('./.tmp/.hidden');

      expect(fileStats).to.exist;
      expect(fileStats.errorCode).to.equal(0);
      expect(fileStats).to.have.property('isHidden', true);
    });

    it('returns correct stats for a read only file', function () {
      var fileStats = file.getFileStats('./.tmp/readOnly');

      expect(fileStats).to.exist;
      expect(fileStats.errorCode).to.equal(0);
      expect(fileStats).to.have.property('isReadOnly', true);
    });

    it('returns correct stats for a directory', function () {
      var fileStats = file.getFileStats('./.tmp');

      expect(fileStats).to.exist;
      expect(fileStats.errorCode).to.equal(0);
      expect(fileStats).to.have.property('isDirectory', true);
    });

    it('returns correct stats for a system file', function () {
      var fileStats = file.getFileStats('./.tmp/systemFile');

      expect(fileStats).to.exist;
      expect(fileStats.errorCode).to.equal(0);
      expect(fileStats).to.have.property('isSystem', true);
    });

    it('returns an error for non existant file', function () {
      var fileStats = file.getFileStats('./.tmp/nonExistant.txt');

      expect(fileStats).to.exist;
      expect(fileStats.errorCode).to.equal(2);
    });
  })
});