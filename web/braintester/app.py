from flask import Flask, render_template, request, abort, Response, redirect, url_for, session, render_template_string,send_from_directory

app = Flask(__name__)
app.secret_key = 'supersecretkey'  

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/hibro/login/', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        username = request.form.get('username')
        password = request.form.get('password')
        if username == 'MarcusovGHGSCCFV123@gmail.com' and password == 'elaine':
            session['logged_in'] = True
            if request.headers.get('x-real-ip') == '127.0.0.1':
                return redirect(url_for('admin')) 
            return Response("""\
                <html>
                <head>
                    <title>403 Forbidden</title>
                    <style>
                        body {
                            font-family: Arial, sans-serif;
                            text-align: center;
                            margin-top: 20%;
                            color: #333;
                        }
                        h1 {
                            font-size: 3em;
                            color: red;
                        }
                        p {
                            font-size: 0.9em;
                            color: #666;
                        }
                    </style>
                </head>
                <body>
                    <h1>F0R8IDD3N</h1>
                    <p>maybe bypass?</p>
                </body>
                </html>
            """, status=403, mimetype='text/html')
        else:
            abort(404) 
    return render_template('login.html')

@app.route('/admin', methods=['GET', 'POST'])
def admin():
    if not session.get('logged_in'):
        abort(403) 
    if request.method == 'POST':
        post_content = request.form.get('content', "")
        if "${" in post_content:
            processed_content = post_content.replace("${", "{{").replace("}", "}}")
        else:
            processed_content = post_content 
        return render_template_string(f"""
            <!DOCTYPE html>
            <html lang="en">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                <title>Admin Panel</title>
                <link rel="stylesheet" href="/static/style3.css">
            </head>
            <body>
                <div class="admin-container">
                    <h1>Admin Panel</h1>
                    <form method="POST">
                        <div class="form-group">
                            <label for="content">Create a Post:</label>
                            <textarea id="content" name="content" rows="5" placeholder="Write your post here..."></textarea>
                        </div>
                        <button type="submit" class="submit-btn">Submit</button>
                    </form>
                    <div class="post-preview">
                        <h2>Post Preview:</h2>
                        <p>{processed_content}</p>
                    </div>
                </div>
            </body>
            </html>
        """)  
    return render_template('admin.html', content="")




@app.route('/robots.txt')
def robots():
    return send_from_directory('static', 'robots.txt')

if __name__ == '__main__':
    app.run()
