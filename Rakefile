# encoding: utf-8

require 'git'
require 'rake'
require 'rubygems'
require 'rake/version_task'         # gem install version
require 'version'

# requires additional packages on MacOS (including Homebrew):
# $ /usr/bin/ruby -e "$(curl -fsSL \
#   https://raw.githubusercontent.com/Homebrew/install/master/install)"
# $ brew install doxygen        # generates documentation from source code
# $ brew cask install mactex    # MacTeX

Rake::VersionTask.new do |task|
  # prevent auto-commit on version bump
  task.with_git = false
end

# adjust as appropriate
DOXYFILE        = 'Doxyfile'
GITHUB_USERNAME = '4-20ma'
GITHUB_REPO     = 'i2c_adc_ads7828'
HEADER_FILE     = "#{GITHUB_REPO}.h"
HISTORY_FILE    = 'HISTORY.markdown'
PROPERTIES_FILE = 'library.properties'
VERSION_FILE    = Version.version_file('').basename.to_s


task :default => :info

desc 'Display instructions for public release'
task :info do
  puts <<-EOF.gsub(/^\s{2}/, '')
  
  Instructions for public release
  
  - Update version, as appropriate:
  
    $ rake version:bump           # or
    $ rake version:bump:minor     # or
    $ rake version:bump:major     # or
    edit 'VERSION' file directly
    
  - Prepare release date, 'HISTORY.markdown' file, documentation:
  
    $ rake prepare
    
  - Review changes to 'HISTORY.markdown' file
    This file is assembled using git commit messages; review for completeness.
  
  - Review html documentation files
    These files are assembled using source code Doxygen tags; review for
    for completeness.
  
  - Add & commit source files, tag, push to origin/master;
    add & commit documentation files, push to origin/gh-pages:
  
    $ rake release
    
  EOF
end # task :info


desc 'Prepare HISTORY file for release'
task :prepare => 'prepare:default'

namespace :prepare do
  task :default => %w(release_date library_properties history documentation)
  
  desc 'Prepare documentation'
  task :documentation => :first_time do
    version = Version.current.to_s
    
    # update parameters in Doxyfile
    cwd = File.expand_path(__dir__)
    file = File.join(cwd, 'doc', DOXYFILE)
    
    contents = IO.read(file)
    contents.sub!(/(^PROJECT_NUMBER\s*=)(.*)$/) do |match|
      "#{$1} v#{version}"
    end # contents.sub!(...)
    IO.write(file, contents)
    
    # chdir to doc/ and call doxygen to update documentation
    Dir.chdir(to = File.join(cwd, 'doc'))
    system('doxygen', DOXYFILE)
    
    # chdir to doc/latex and call doxygen to update documentation
    Dir.chdir(from = File.join(cwd, 'doc', 'latex'))
    system('make')
    
    # move/rename file to 'extras/GITHUB_REPO reference-x.y.pdf'
    to = File.join(cwd, 'extras')
    FileUtils.mv(File.join(from, 'refman.pdf'),
      File.join(to, "#{GITHUB_REPO} reference-#{version}.pdf"))
  end # task :documentation
  
  # desc 'Prepare doc/html directory (first-time only)'
  task :first_time do
    cwd = File.expand_path(File.join(__dir__, 'doc', 'html'))
    FileUtils.mkdir_p(cwd)
    Dir.chdir(cwd)
    
    # skip if this operation has already been completed
    next if 'refs/heads/gh-pages' == `git config branch.gh-pages.merge`.chomp
    
    # configure git remote/branch options
    origin = "git@github.com_#{GITHUB_USERNAME}:#{GITHUB_USERNAME}/" +
      "#{GITHUB_REPO}.git"
    `git init`
    `git remote add origin #{origin}`
    `git checkout --orphan gh-pages`
    `git config --replace-all branch.gh-pages.remote origin`
    `git config --replace-all branch.gh-pages.merge refs/heads/gh-pages`
    `touch index.html`
    `git add .`
    `git commit -a -m 'Initial commit'`
  end
  
  desc 'Prepare release history'
  task :history, :tag do |t, args|
    cwd = File.expand_path(__dir__)
    g = Git.open(cwd)
    
    current_tag = args[:tag] || Version.current.to_s
    prior_tag = g.tags.last
    
    history = "## [v#{current_tag} (#{Time.now.strftime('%Y-%m-%d')})]"
    history << "(/#{GITHUB_USERNAME}/#{GITHUB_REPO}/tree/v#{current_tag})\n"
    
    commits = prior_tag ? g.log.between(prior_tag) : g.log
    history << commits.map do |commit|
      "- #{commit.message}"
    end.join("\n")
    history << "\n\n---\n"
    
    file = File.join(cwd, HISTORY_FILE)
    puts "Updating file #{file}:"
    puts history
    contents = IO.read(file)
    IO.write(file, history << contents)
  end # task :history
  
  desc 'Update version in library properties file'
  task :library_properties do
    version = Version.current.to_s

    cwd = File.expand_path(__dir__)
    file = File.join(cwd, PROPERTIES_FILE)

    contents = IO.read(file)
    contents.sub!(/(version=\s*)(.*)$/) do |match|
      "#{$1}#{version}"
    end # contents.sub!(...)
    IO.write(file, contents)
  end # task :library_properties

  desc 'Update release date in header file'
  task :release_date do
    cwd = File.expand_path(__dir__)
    file = File.join(cwd, 'src', HEADER_FILE)
    
    contents = IO.read(file)
    contents.sub!(/(\\date\s*)(.*)$/) do |match|
      "#{$1}#{Time.now.strftime('%-d %b %Y')}"
    end # contents.sub!(...)
    IO.write(file, contents)
  end # task :release_date
  
end # namespace :prepare


desc 'Release source & documentation'
task :release => 'release:default'

namespace :release do
  task :default => [:source, :documentation]
  
  desc 'Commit documentation changes related to version bump'
  task :documentation do
    version = Version.current.to_s
    cwd = File.expand_path(File.join(__dir__, 'doc', 'html'))
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
    `git add \
      doc/#{DOXYFILE} \
      "extras/#{GITHUB_REPO} reference-#{version}.pdf" \
      src/#{HEADER_FILE} \
      #{HISTORY_FILE} \
      #{PROPERTIES_FILE} \
      #{VERSION_FILE} \
    `
    `git commit -m 'Version bump to v#{version}'`
    `git tag -a -f -m 'Version v#{version}' v#{version}`
    `git push origin master`
    `git push --tags`
  end # task :source
  
end # namespace :release
