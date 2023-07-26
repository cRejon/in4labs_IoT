import os

from flask import current_app


def create_navtab(board):
    navtab_html = f'''
        <button class="nav-link col-sm-4" id="nav-{board}-tab" data-bs-toggle="tab" data-bs-target="#nav-{board}" type="button" role="tab" aria-controls="nav-{board}" aria-selected="true">{board}</button>
    '''

    return navtab_html

def fill_examples(board):
    examples_path = os.path.join(current_app.instance_path, 'examples', board)
    examples = [f for f in os.listdir(examples_path) if os.path.isfile(os.path.join(examples_path, f))]
    # Convert example names to show spaces instead of underscores and remove .ino extension
    examples_name = [example.replace('_', ' ').replace('.ino', '') for example in examples]

    examples_html = ''
    for example, example_name in zip(examples, examples_name):
        examples_html += f'<option value="{example}">{example_name}</option>\n'
    return examples_html

def create_editor(board):
    examples = fill_examples(board)

    editor_html = f'''
                <div class="tab-pane fade active show" id="nav-{board}" role="tabpanel" aria-labelledby="nav-{board}-tab">
                    <div class="editor-nav">
                        <div class="row">
                            <div class="editor-examples col-sm-4">
                                <div class="btn-group editor-examples-dropdown">
                                    <select class="editor-select" id="editor-select-{board}" onchange="onLoadExample('{board}',this.value)">
                                        <option value="new" selected="selected">New Sketch</option>
                                        {examples}
                                    </select>
                                </div>
                            </div>
                            <div class="editor-cta col-sm-8">
                                <div class="editor-cta-load">
                                    <button class="upload" onclick="document.getElementById('file-input-{board}').click()" data-toggle="tooltip" data-placement="top" title="Load File"><span class="fa fa-upload"/></button>
                                    <input id="file-input-{board}" type="file" accept=".ino" style="display: none;" />
                                    <script>
                                        document.getElementById('file-input-{board}').addEventListener('change', onLoadFile, false);
                                    </script>
                                    <button class="download" onclick="onSaveFile('{board}')" data-toggle="tooltip" data-placement="top" title="Save File"><span class="fa fa-download"/></button>
                                </div>
                                <div class="editor-cta-compile">
                                    <button class="compile"
                                            onclick="onCompileCode('{board}')"
                                            data-toggle="tooltip"
                                            data-placement="top"
                                            title="Compile code">
                                        <span class="fa fa-check"/>
                                    </button>
                                    <button class="execute"
                                            id="button-execute-{board}"
                                            onclick="onExecuteCode('{board}')"
                                            data-toggle="tooltip"
                                            data-placement="top"
                                            title="Run"
                                            disabled="disabled">
                                        <span class="fa fa-play-circle"/>
                                    </button>
                                    <button class="stop"
                                            id="button-stop-{board}"
                                            onclick="onStopExecution('{board}')"
                                            data-toggle="tooltip"
                                            data-placement="top"
                                            title="Stop"
                                            disabled="disabled">
                                        <span class="fa fa-stop-circle"/>
                                    </button>
                                </div>
                            </div>
                        </div>
                    </div>
                    <form id="editor-{board}">
                        <textarea id="text-{board}" name="text-{board}"></textarea>
                        <script>
                            let editor_{board} = CodeMirror.fromTextArea(document.getElementById('text-{board}'), {{
                                mode: 'text/x-c++src',
                                theme: 'neat',
                                lineNumbers: true,
                                autoCloseBrackets:true,
                            }});

                            function {board}GetEditor() {{
                                return editor_{board};
                            }}

                            onLoadExample('{board}','new');

                            // Listener to trigger a function when the code changes
                            editor_{board}.on("change", function() {{ onChangeCode('{board}') }});
                        </script>
                    </form>
                </div>
    '''

    return editor_html
