from fastapi import FastAPI
from routes.devices import router as devices_router
from routes.users import router as users_router
from routes.values import router as values_router
from routes.Log_In import router as Log_In_router
from database import database

app = FastAPI()

app.include_router(devices_router, prefix="/devices")
app.include_router(users_router, prefix="/users")
app.include_router(values_router, prefix="/values")
app.include_router(Log_In_router, prefix="/Log_in")

@app.get("/")
async def root():
	return {"message": "welcome, this is a api for the school project Integration"}

@app.on_event("startup")
async def startup():
    await database.connect()

@app.on_event("shutdown")
async def shutdown():
    await database.disconnect()
