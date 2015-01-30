var fs = require('fs');
var through2 = require('through2');
var binding = require('./lib/binding');

module.exports = function() {
  var fd = binding.init();
  var watches = {};
  var readStream = fs.createReadStream(null, {
    fd: fd
  });

  var res = readStream.pipe(through2.obj(function(buf, _, cb) {
    this.push(binding.parseEvent(buf));
    cb();
  }));

  res.close = function() {
    readStream.close();
  };

  res.addWatch = function(pathname, mask) {
    if (watches[pathname]) return;
    var wd = binding.addWatch(fd, pathname, mask);
    watches[pathname] = wd;
  };

  res.removeWatch = function(pathname) {
    var wd = watches[pathname];
    if (wd == null) return;
    binding.rmWatch(fd, wd);
    delete watches[pathname];
  };

  return res;
}
