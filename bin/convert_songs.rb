#!/usr/bin/env ruby

require "rake"

class String
  def sanitise
    self.
      delete("\n").
      gsub("ä", "ae").gsub("ä", "ae").
      gsub("ö", "oe").
      gsub("ü", "ue").gsub("ü", "ue").
      gsub("ß", "ss").
      tr_s("^a-z0-9\-.", "_")
  end
end

def bestname(path)
  dir  = File.dirname(path)
  name = File.basename(path)
  basename = name.downcase.sanitise

  i = (basename[/^\d+/] || 1).to_i

  basename.gsub!(/^\d+[\-_]*/, "")

  name = sprintf("%04d", i) + "_" + basename

  while File.exist?(File.join(dir, name))
    i += 1
    name = sprintf("%04d", i) + "_" + basename
  end

  File.join(dir, name)
end

def main
  path = ARGV.shift

  files = Dir.glob(File.join(path, "*.m4a"))

  files.sort.each do |file|
    newname = bestname(file.ext(".mp3"))
    puts "Converting #{file} -> #{newname}"
    system "ffmpeg", \
      "-i", file, \
      "-acodec", "libmp3lame", \
      "-aq", "2", \
      newname
  end
end

main
