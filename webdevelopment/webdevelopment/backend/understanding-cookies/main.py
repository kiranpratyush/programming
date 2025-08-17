from fastapi import FastAPI,Request
from fastapi.responses import HTMLResponse
from fastapi.staticfiles import StaticFiles
from fastapi.templating import Jinja2Templates

app = FastAPI()

app.mount("/static",StaticFiles(directory='static'),name="static")

templates = Jinja2Templates(directory="templates")


@app.get("/",response_class=HTMLResponse)
def get_root(request:Request):
    print(request.cookies)
    response = templates.TemplateResponse(request,"index.html",media_type="text/html",status_code=200)
    response.set_cookie("first-cookie","123")
    response.set_cookie("yummy-cookie","strawberry",max_age=60)
    return response

@app.get("/second",response_class=HTMLResponse)
def get_second(request:Request):
    print(request.cookies)
    response = templates.TemplateResponse(request,"second.html",media_type="text/html",status_code=200)
    response.set_cookie("second-cookie","234",httponly=True,path="/second")
    return response