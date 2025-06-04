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
    check_values = {
        "user_first_name": user_first_name,
        "user_last_name": user_last_name,
        "user_birth_date": user_birth_date,
    }
    insert_values = {
        "user_first_name": user_first_name,
        "user_last_name": user_last_name,
        "user_birth_date": user_birth_date,
        "user_weight": user_weight,
        "user_height": user_height,
    }

    try:
        existing_user = await database.fetch_one(query=check_query, values=check_values)
        if existing_user:
            return {"message": "User already exists"}

        await database.execute(query=insert_query, values=insert_values)
        return {"message": "User created successfully"}

    except Exception as e:
        return {"message": f"Failed to create user: {str(e)}"}


#get all data for debugging
@router.get("/get_all")
async def read_users():
    query = "SELECT * FROM Users"
    try:
        users = await database.fetch_all(query=query)
        return {"users": [dict(user) for user in users]}
    except Exception as e:
        return {"message": "Failed to fetch users", "error": str(e)}

#get data from one user with user_id
@router.get("/get_user")
async def get_user(
    user_first_name: int
):
    query = """
    SELECT * FROM Users
    WHERE user_id = :user_id
    """
    values = {
        "user_id": user_id,
    }

    try:
        users = await database.fetch_all(query=query, values=values)
        return {"users": [dict(user) for user in users]}
    except Exception as e:
        return {"message": "Failed to fetch users", "error": str(e)}


#get bmi with first name, last name and birth date
@router.get("/bmi_measurement")
async def get_bmi_measurement(
    user_id: int
):
    query = """
        SELECT user_weight, user_height
        FROM Users
        WHERE user_id = :user_id
    """
    values = {
        "user_id": user_id
    }

    try:
        user = await database.fetch_one(query=query, values=values)
        if user:
            weight = user["user_weight"]
            height_cm = user["user_height"]
            height_m = height_cm / 100

            if height_m == 0:
                return {"message": "Invalid height (0 cm)"}

            bmi = weight / (height_m ** 2)
            return {
                "bmi": round(bmi, 2)
            }
        else:
            return {"message": "User not found"}
    except Exception as e:
        return {"message": f"Failed to fetch user data: {str(e)}"}

#change weight and heigt of a user_id
@router.get("/update_measurements/")
async def update_user_measurements(
    user_id: int,
    user_weight: int,
    user_height: int
):
    query = """
        UPDATE Users
        SET user_weight = :user_weight,
            user_height = :user_height
        WHERE user_id = :user_id
    """
    values = {
        "user_weight": user_weight,
        "user_height": user_height,
        "user_id": user_id
    }
    await database.execute(query=query, values=values)
    return {"message": "User measurements updated successfully"}
