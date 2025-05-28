from fastapi import APIRouter
from database import database
from pydantic import BaseModel
from datetime import date

router = APIRouter()

class UserCreate(BaseModel):
    user_first_name: str
    user_last_name: str
    user_birth_date: date
    user_weight: int
    user_height: int

@router.get("/create/")
async def create_user_get(
    user_first_name: str, 
    user_last_name: str, 
    user_birth_date: date, 
    user_weight: int, 
    user_height: int
):
    check_query = """
    SELECT 1 FROM Users
    WHERE user_first_name = :user_first_name
      AND user_last_name = :user_last_name
      AND user_birth_date = :user_birth_date
    """
    insert_query = """
    INSERT INTO Users (user_first_name, user_last_name, user_birth_date, user_weight, user_height)
    VALUES (:user_first_name, :user_last_name, :user_birth_date, :user_weight, :user_height)
    """
    values = {
        "user_first_name": user_first_name,
        "user_last_name": user_last_name,
        "user_birth_date": user_birth_date,
        "user_weight": user_weight,
        "user_height": user_height,
    }

    try:
        existing_user = await database.fetch_one(query=check_query, values=values)
        if existing_user:
            return {"message": "User already exists"}

        await database.execute(query=insert_query, values=values)
        return {"message": "User created successfully"}

    except Exception as e:
        return {"message": f"Failed to create user: {str(e)}"}


#get all data
@router.get("/get_all_users")
async def read_users():
    query = "SELECT * FROM Users"
    try:
        users = await database.fetch_all(query=query)
        return {"users": [dict(user) for user in users]}
    except Exception as e:
        return {"message": "Failed to fetch users", "error": str(e)}
    
#get data from one user
@router.get("/get_user")
async def create_user_get(
    user_first_name: str, 
    user_last_name: str, 
    user_birth_date: date, 
):
    query = """
    SELECT * FROM Users
    WHERE user_first_name = :user_first_name
    AND user_last_name = :user_last_name
    AND user_birth_date = :user_birth_date
    """
    values = {
        "user_first_name": user_first_name,
        "user_last_name": user_last_name,
        "user_birth_date": user_birth_date
    }
    
    try:
        users = await database.fetch_all(query=query, values=values)
        return {"users": [dict(user) for user in users]}
    except Exception as e:
        return {"message": "Failed to fetch users", "error": str(e)}
    

#get bmi with first name, last name and birth date
@router.get("/bmi_measurement")
async def get_bmi_measurement(
    user_first_name: str,
    user_last_name: str,
    user_birth_date: date
):
    query = """
        SELECT user_weight, user_height 
        FROM Users
        WHERE user_first_name = :user_first_name
          AND user_last_name = :user_last_name
          AND user_birth_date = :user_birth_date
    """
    values = {
        "user_first_name": user_first_name,
        "user_last_name": user_last_name,
        "user_birth_date": user_birth_date
    }

    try:
        user = await database.fetch_one(query=query, values=values)
        if user:
            return {
                "user_weight": user["user_weight"],
                "user_height": user["user_height"]
            }
        else:
            return {"message": "User not found"}
    except Exception as e:
        return {"message": f"Failed to fetch user data: {str(e)}"}
