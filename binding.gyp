{
  'targets': [
    {
      'target_name': 'inotify2',
      'include_dirs': ["<!(node -e \"require('nan')\")"],
      'sources': [
        'src/inotify.cc',
      ],
      'cflags': [
        '-std=c++0x',
      ],
    }
  ]
}
