#include <nan.h>
#include <sys/inotify.h>

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;
using v8::Number;

NAN_METHOD(Init) {
  NanScope();

  auto res = inotify_init();
  if (res < 0) {
    NanThrowError(NanNew<String>(strerror(errno)));
  }

  NanReturnValue(NanNew<Number>(res));
}

NAN_METHOD(AddWatch) {
  NanScope();

  auto fd = args[0]->Int32Value();
  String::Utf8Value pathname(args[1]);
  auto mask = args[2]->Uint32Value();

  auto res = inotify_add_watch(fd, *pathname, mask);
  if (res < 0) {
    NanThrowError(NanNew<String>(strerror(errno)));
  }

  NanReturnValue(NanNew<Number>(res));
}

NAN_METHOD(RmWatch) {
  NanScope();

  auto fd = args[0]->Uint32Value();
  auto wd = args[1]->Uint32Value();

  auto res = inotify_rm_watch(fd, wd);
  if (res < 0) {
    NanThrowError(NanNew<String>(strerror(errno)));
  }

  NanReturnUndefined();
}

NAN_METHOD(ParseEvent) {
  NanScope();

  auto event = (inotify_event*)node::Buffer::Data(args[0].As<Object>());
  auto res = NanNew<Object>();

  res->Set(NanNew<String>("wd"), NanNew<Number>(event->wd));
  res->Set(NanNew<String>("mask"), NanNew<Number>(event->mask));
  res->Set(NanNew<String>("cookie"), NanNew<Number>(event->cookie));
  if (event->len) {
    res->Set(NanNew<String>("name"), NanNew<String>(event->name));
  }

  NanReturnValue(res);
}

void InitModule(Handle<Object> exports) {
  exports->Set(NanNew<String>("init"),
    NanNew<FunctionTemplate>(Init)->GetFunction());
  exports->Set(NanNew<String>("addWatch"),
    NanNew<FunctionTemplate>(AddWatch)->GetFunction());
  exports->Set(NanNew<String>("rmWatch"),
    NanNew<FunctionTemplate>(RmWatch)->GetFunction());
  exports->Set(NanNew<String>("parseEvent"),
    NanNew<FunctionTemplate>(ParseEvent)->GetFunction());

  auto constants = NanNew<Object>();
  exports->Set(NanNew<String>("constants"), constants);

  NODE_DEFINE_CONSTANT(constants, IN_ACCESS); //File was accessed (read)
  NODE_DEFINE_CONSTANT(constants, IN_ATTRIB); //Metadata changed, e.g., permissions, timestamps,
                                              //extended attributes, link count (since Linux 2.6.25),
                                              //UID, GID, etc.
  NODE_DEFINE_CONSTANT(constants, IN_CLOSE_WRITE); //File opened for writing was closed
  NODE_DEFINE_CONSTANT(constants, IN_CLOSE_NOWRITE); //File not opened for writing was closed
  NODE_DEFINE_CONSTANT(constants, IN_CREATE); //File/directory created in watched directory
  NODE_DEFINE_CONSTANT(constants, IN_DELETE); //File/directory deleted from watched directory
  NODE_DEFINE_CONSTANT(constants, IN_DELETE_SELF); //Watched file/directory was itself deleted
  NODE_DEFINE_CONSTANT(constants, IN_MODIFY); //File was modified
  NODE_DEFINE_CONSTANT(constants, IN_MOVE_SELF); //Watched file/directory was itself moved
  NODE_DEFINE_CONSTANT(constants, IN_MOVED_FROM); //File moved out of watched directory
  NODE_DEFINE_CONSTANT(constants, IN_MOVED_TO); //File moved into watched directory
  NODE_DEFINE_CONSTANT(constants, IN_OPEN); //File was opened
  NODE_DEFINE_CONSTANT(constants, IN_IGNORED); // Watch was removed explicitly (inotify.watch.rm) or
                                               //automatically (file was deleted, or file system was
                                               //unmounted)
  NODE_DEFINE_CONSTANT(constants, IN_ISDIR); //Subject of this event is a directory
  NODE_DEFINE_CONSTANT(constants, IN_Q_OVERFLOW); //Event queue overflowed (wd is -1 for this event)
  NODE_DEFINE_CONSTANT(constants, IN_UNMOUNT); //File system containing watched object was unmounted
  NODE_DEFINE_CONSTANT(constants, IN_ALL_EVENTS);

  NODE_DEFINE_CONSTANT(constants, IN_ONLYDIR); // Only watch the path if it is a directory.
  NODE_DEFINE_CONSTANT(constants, IN_DONT_FOLLOW); // Do not follow a sym link
  NODE_DEFINE_CONSTANT(constants, IN_ONESHOT); // Only send event once
  NODE_DEFINE_CONSTANT(constants, IN_MASK_ADD); //Add (OR) events to watch mask for this pathname if it
                                                //already exists (instead of replacing mask).

  NODE_DEFINE_CONSTANT(constants, IN_CLOSE); // (IN_CLOSE_WRITE | IN_CLOSE_NOWRITE)  Close
  NODE_DEFINE_CONSTANT(constants, IN_MOVE);  //  (IN_MOVED_FROM | IN_MOVED_TO)  Moves

}

NODE_MODULE(inotify2, InitModule)
