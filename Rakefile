require 'rake/clean'

task :default => 'triangles'

desc 'Compile the file and run it'
task :run => 'triangles' do
  sh './triangles'
end

# C version
soil = Dir['lib/SOIL/*.c'].map do |c|
  Struct.new(:c, :o).new(c, c.sub(/\.c$/, '.o'))
end

soil.map(&:to_a).+([ ['triangles.c', 'triangles.o'] ]).each do |c, o|
  CLEAN << o
  file o => c do |t|
    warnings = %w[ all extra no-unused-parameter ].map { |w| '-W' + w }
    includes = %w[ /usr/local/include ./include ].map { |i| '-I' + i }

    sh 'clang', '-cc1', '-emit-obj', '-o', t.name, '-std=c11', *warnings, *t.prerequisites, *includes
  end
end

CLOBBER << 'SOIL.a'
file 'SOIL.a' => soil.map(&:o) do |t|
  sh 'ar', '-r', t.name, *t.prerequisites
end

CLOBBER << 'triangles'
file 'triangles' => [ 'triangles.o', 'SOIL.a' ] do |t|
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

  sh 'ld', '-o', t.name, *before_crt, *t.prerequisites, *paths, *libraries, *after_crt
end



# C++ version
CLEAN << 'trianglespp.o'
file 'trianglespp.o' => 'triangles.cpp' do |t|
  warnings = %w[ all extra no-unused-parameter ].map { |w| '-W' + w }
  includes = %w[ /usr/include/c++/v1 /usr/local/include ].map { |i| '-I' + i }

  sh 'clang++', '-cc1', '-emit-obj', '-o', t.name, '-std=c++11', *warnings, *t.prerequisites, *includes
end

CLOBBER << 'trianglespp'
file 'trianglespp' => 'trianglespp.o' do |t|
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
