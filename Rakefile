# encoding: utf-8

require 'git'
require 'rake'
require 'rubygems'
require 'rake/version_task'         # gem install version
require 'version'

Rake::VersionTask.new do |task|
  # prevent auto-commit on version bump
  task.with_git = false
end

# adjust as appropriate
GITHUB_USERNAME = '4-20ma'
GITHUB_REPO     = 'i2c_adc_ads7828'
HEADER_FILE     = "#{GITHUB_REPO}.h"
HISTORY_FILE    = 'HISTORY.markdown'
VERSION_FILE    = Version.version_file('').basename.to_s


desc 'Prepare HISTORY file for release'
task :prepare => 'prepare:default'

namespace :prepare do
  task :default => [:release_date, :history, :documentation]
  
  desc 'Update documentation'
  task :documentation do
    # chdir to doc/ and call doxygen to update documentation
    Dir.chdir('doc')
    system('doxygen', 'Doxyfile')
  end # task :documentation
  
  desc 'Update release history'
  task :history, :tag do |t, args|
    cwd = File.expand_path(File.dirname(__FILE__))
    g = Git.open(cwd)
    
    current_tag = args[:tag] || Version.current.to_s
    prior_tag = g.tags.last   # may be nil
    
    history = "## [v#{current_tag} (#{Time.now.strftime('%Y-%m-%d')})]"
    history << "(/#{GITHUB_USERNAME}/#{GITHUB_REPO}/tree/v#{current_tag})\n"
    
    # TODO: handle case where prior_tag is nil; log.between(nil) fails
    history << g.log.between(prior_tag).map do |commit|
      "- #{commit.message}"
    end.join("\n")
    history << "\n\n---\n"
    
    file = File.join(cwd, HISTORY_FILE)
    puts "Updating file #{file}:"
    puts history
    contents = IO.read(file)
    IO.write(file, history << contents)
  end # task :history
  
  desc 'Update release date in header file'
  task :release_date do
    cwd = File.expand_path(File.dirname(__FILE__))
    file = File.join(cwd, HEADER_FILE)
    
    contents = IO.read(file)
    contents.sub!(/(\\date\s*)(.*)$/) do |match|
      "#{$1}#{Time.now.strftime('%-d %b %Y')}"
    end # contents.sub!(...)
    IO.write(file, contents)
  end # task :release_date
  
end # namespace :prepare


desc 'Release'
task :release => 'release:default'

namespace :release do
  task :default => [:source, :documentation]
  
  desc 'Commit documentation changes related to version bump'
  task :documentation do
    version = Version.current.to_s
    cwd = File.expand_path(File.join(File.dirname(__FILE__), 'doc', 'html'))
    g = Git.open(cwd)
    
    # `git add .`
    g.add
    
    # remove each deleted item
    g.status.deleted.each do |item|
      g.remove(item[0])
    end # g.status.deleted.each
    
    # commit changes if items added, changed, or deleted
    if g.status.added.size > 0 || g.status.changed.size > 0 ||
      g.status.deleted.size > 0 then
      message = "Update documentation for v#{version}"
      puts g.commit(message)
    else
      puts "No changes to commit v#{version}"
    end # if g.status.added.size > 0 || g.status.changed.size > 0...
    
    g.push('origin', 'gh-pages')
  end # task :documentation
  
  desc 'Commit source changes related to version bump'
  task :source do
    version = Version.current.to_s
    `git add #{HEADER_FILE} #{HISTORY_FILE} #{VERSION_FILE}`
    `git commit -m 'Version bump to v#{version}'`
    `git tag -a -f -m 'Version v#{version}' v#{version}`
    `git push origin master`
    `git push --tags`
  end # task :source
  
end # namespace :release
