require 'rake/clean'

task :default => 'triangles'

desc 'Compile the file and run it'
task :run => 'triangles' do
  sh './triangles'
end

file 'triangles.o' => 'triangles.cpp' do |t|
  warnings = %w[ all extra no-unused-parameter ].map { |w| '-W' + w }
  includes = %w[ /usr/include/c++/v1 /usr/local/include ].map { |i| '-I' + i }

  sh 'clang++', '-cc1', '-emit-obj', '-o', t.name, '-std=c++11', *warnings, *t.prerequisites, *includes
end
CLEAN << 'triangles.o'

file 'triangles' => 'triangles.o' do |t|
  libraries = [
    'c',       # C standard library
    'm',       # C math library
    'c++',     # C++ standard library
    'gcc_s',   # GCC C++ runtime library

    'GL',
    'GLEW',
    'glfw3',

    'X11',
    'Xxf86vm', # XFree86 Video Mode Extension Library
    'Xcursor', # Cursor management library
    'Xi',      # XInput Device Extension Library
    'Xinerama',
    'Xrandr',
  ].map { |l| '-l' + l }
  paths = %w[ /usr/local/lib ].map { |l| '-L' + l }
  before_crt = %w[ /usr/lib/crt1.o /usr/lib/crti.o /usr/lib/crtbegin.o ]
  after_crt = %w[ /usr/lib/crtend.o /usr/lib/crtn.o ]

  sh 'ld', '-o', t.name, *before_crt, *t.prerequisites, *paths, *libraries, *after_crt
end
CLOBBER << 'triangles'
