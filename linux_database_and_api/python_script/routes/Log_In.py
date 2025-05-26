from fastapi import APIRouter
from database import database
router = APIRouter()

@router.get("/")
async def get_Log_In():
    return {"message":"hello from routes/Log_In"}

