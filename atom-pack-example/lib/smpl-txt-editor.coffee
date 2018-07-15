{ CompositeDisposable } = require 'atom'

module.exports = SmplEditor =
  subscriptions: null

  activate: ->
    @subscriptions = new CompositeDisposable
    @subscriptions.add atom.commands.add 'atom-workspace', 'smpl-txt-editor:to_upper': => @to_upper()
    @subscriptions.add atom.commands.add 'atom-workspace', 'smpl-txt-editor:to_lower': => @to_lower()
    @subscriptions.add atom.commands.add 'atom-workspace', 'smpl-txt-editor:inverse': => @inverse()
    @subscriptions.add atom.commands.add 'atom-workspace', 'smpl-txt-editor:q_double': => @wrap_with('"')
    @subscriptions.add atom.commands.add 'atom-workspace', 'smpl-txt-editor:q_single': => @wrap_with("'")
    @subscriptions.add atom.commands.add 'atom-workspace', 'smpl-txt-editor:wrap-slash': => @wrap_with("/")
    @subscriptions.add atom.commands.add 'atom-workspace', 'smpl-txt-editor:wrap-bslash': => @wrap_with("\\")

  deactivate: ->
    @subscriptions.dispose()


  to_upper: () ->
    if editor = atom.workspace.getActiveTextEditor()
      selection = editor.getLastSelection()
      replacement = selection.getText().toUpperCase()
      selection.insertText(replacement)

  to_lower: () ->
    if editor = atom.workspace.getActiveTextEditor()
      selection = editor.getLastSelection()
      replacement = selection.getText().toLowerCase()
      selection.insertText(replacement)

  inverse: () ->
    if editor = atom.workspace.getActiveTextEditor()
      selection = editor.getLastSelection()
      replacement = selection.getText().split("").reverse().join("")
      selection.insertText(replacement)

  wrap_with: (ch) ->
    if editor = atom.workspace.getActiveTextEditor()
      selection = editor.getLastSelection()
      replacement = @wrap(selection.getText(), ch)
      selection.insertText(replacement)

  wrap: (text, ch) ->
    re = /(?:['"]?)(\w+(?:\s+\w+)*)(?:['"]?)/g
    text.replace(re, ch + '$1' + ch)
