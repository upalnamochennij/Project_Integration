from fastapi import APIRouter
from database import database
from pydantic import BaseModel
from datetime import date

router = APIRouter()

class LogInCreate(BaseModel):
    user_id: int
    username: str
    password: str

#save password and username
@router.get("/create_Log_In")
async def create_Log_In(
    user_id: int,
    username: str,
    password: str
):
    
    values = {
        "user_id": user_id,
        "Login_name": username,
        "Login_password": password
        }

    insert_query = """
    INSERT INTO Log_In (user_id, Login_name, Login_password)
    VALUES (:user_id, :Login_name, :Login_password)
    """
    try:
        await database.execute(query=insert_query, values=values)
        return {"message": "Log in credentials added successfully"}
    except Exception as e:
        return {"message": f"Failed to add log in data: {str(e)}"}

#get password with username
@router.get("/get_password_by_username")
async def get_password_by_username(username: str):
    query = "SELECT Login_password FROM Log_In WHERE Login_name = :username"
    result = await database.fetch_one(query=query, values={"username": username})

    if result:
        return {"username": username, "password": result["Login_password"]}
    return {"message": "Username not found"}

#get password with user_id
@router.get("/get_password_by_user_id")
async def get_password_by_user_id(user_id: int):
    query = "SELECT Login_password FROM Log_In WHERE user_id = :user_id"
    result = await database.fetch_one(query=query, values={"user_id": user_id})

    if result:
        return {"user_id": user_id, "password": result["Login_password"]}
    return {"message": "User ID not found"}

#check if username exists (already)
@router.get("/check_username_exists")
async def check_username_exists(username: str):
    query = "SELECT 1 FROM Log_In WHERE Login_name = :username"
    result = await database.fetch_one(query=query, values={"username": username})

    return {"exists": bool(result)}
