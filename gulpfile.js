'use strict';

/** Regular npm dependendencies */
var gulp = require('gulp');
var stylish = require('jshint-stylish');
var lazypipe = require('lazypipe');

/** Gulp dependencies */
var jshint = require('gulp-jshint');
var mocha = require('gulp-mocha');

/** Grab-bag of paths */
var paths = {
  code: ['./lib/**/*.js', '!./lib/**/*.spec.js'],
  test: './lib/**/*.spec.js',
  gulpfile: './gulpfile.js'
};

/** Reusable functions */

function jshintPipe(jshintrc) {
  return lazypipe()
    .pipe(jshint, jshintrc)
    .pipe(jshint.reporter, stylish)
    .pipe(jshint.reporter, 'fail')();
}

/** Gulp tasks */
gulp.task('default', ['test']);

gulp.task('test', ['jshint', 'mocha']);

gulp.task('jshint', function () {
  return gulp.src(paths.code)
    .pipe(jshintPipe('.jshintrc'));
});

gulp.task('jshint:test', function () {
  return gulp.src(paths.test)
    .pipe(jshintPipe('.spec.jshintrc'));
});

gulp.task('jshint:gulpfile', function () {
  return gulp.src(paths.gulpfile)
    .pipe(jshintPipe('.jshintrc'));
});

gulp.task('jshint:all', ['jshint', 'jshint:test', 'jshint:gulpfile']);

gulp.task('mocha', function() {
  return gulp.src(paths.test, { read: false })
    .pipe(mocha({ reporter: 'nyan' }));
});