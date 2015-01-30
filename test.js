var inotify = require('./');

var w = new inotify();
w.addWatch('.', inotify.constants.IN_ACCESS, function(e) {
  console.dir(e);
});
