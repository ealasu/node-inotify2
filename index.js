var fs = require('fs');
var through2 = require('through2');
var binding = require('./build/Release/inotify');

module.exports = INotify;
module.exports.constants = binding.constants;

function INotify() {
  var self = this;
  this._watches = {};
  this._fd = binding.init();
  this._readStream = fs.createReadStream(null, {fd: this._fd});
  this._readStream.pipe(through2.obj(function(buf, _, cb) {
    var e = binding.parseEvent(buf);
    self._watches[e.wd](e);
    cb();
  }));
}

INotify.prototype.close = function() {
  this._readStream.close();
};

INotify.prototype.addWatch = function(pathname, mask, callback) {
  if (mask == null) {
    mask = binding.constants.IN_ALL_EVENTS;
  }
  var wd = binding.addWatch(this._fd, pathname, mask);
  this._watches[wd] = callback;
  return wd;
};

INotify.prototype.removeWatch = function(wd) {
  binding.rmWatch(this._fd, wd);
  delete this._watches[wd];
};
