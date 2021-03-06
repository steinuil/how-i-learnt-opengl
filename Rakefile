require 'rake/clean'

task :default => 'todd-cube'

desc 'Compile the file and run it'
task :run => 'todd-cube' do
  sh './todd-cube'
end

soil = Dir['lib/SOIL/*.c'].map do |c|
  Struct.new(:c, :o).new(c, c.sub(/\.c$/, '.o'))
end

soil.map(&:to_a).+([ [ ['todd-cube.c', 'include/vec.h'], 'todd-cube.o'] ]).each do |c, o|
  CLEAN << o
  file o => c do |t|
    warnings = %w[ all extra no-unused-parameter no-sign-compare no-missing-braces ].map { |w| '-W' + w }
    includes = %w[ /usr/local/include ./include ].map { |i| '-I' + i }

    sh 'clang', '-cc1', '-emit-obj', '-o', t.name, '-std=c11', *warnings, t.prerequisites[0], *includes
  end
end

CLOBBER << 'SOIL.a'
file 'SOIL.a' => soil.map(&:o) do |t|
  sh 'ar', 'cr', t.name, *t.prerequisites
end

CLEAN << 'todd-cube.core'
CLOBBER << 'todd-cube'
file 'todd-cube' => [ 'todd-cube.o', 'SOIL.a' ] do |t|
  libraries = [
    'c',       # C standard library
    'm',       # C math library

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
  before_crt = %w[ /usr/lib/crt1.o /usr/lib/crti.o ]
  after_crt = %w[ /usr/lib/crtn.o ]

  sh 'ld', '-o', t.name, *before_crt, 'todd-cube.o', 'SOIL.a', *paths, *libraries, *after_crt
end

CLOBBER << 'include/vec.h'
file 'include/vec.h' => 'include/vec.h.erb' do |t|
  require 'erb'
  puts 'erb include/vec.h.erb -o include/vec.h'
  File.write t.name, ERB.new(File.read(t.prerequisites[0]), nil, '-').result
end

#clang -shared -o libtest.so -fPIC test.c
