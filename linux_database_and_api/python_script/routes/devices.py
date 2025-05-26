from fastapi import APIRouter
from database import database
from pydantic import BaseModel
from datetime import date

router = APIRouter()

class DeviceCreate(BaseModel):
    user_id: int
    registration_date: date
    
#create device
@router.get("/create_device")
async def create_device_(
    user_id: int,
    registration_date: date
):
    query = """
    INSERT INTO Devices (user_id, registration_date)
    SELECT :user_id, :registration_date
    WHERE EXISTS (
        SELECT 1 FROM Users
        WHERE user_id = :user_id
    )AND NOT EXISTS (
    SELECT 1 FROM Devices
    WHERE user_id = :user_id AND registration_date = :registration_date
    );
    """
    values = {
        "user_id": user_id,
        "registration_date": registration_date,
        
    }
    try:
        await database.execute(query=query, values=values)
        return {"message": "device created or already exists"}
    except Exception as e:
        return {"message": f"Failed to create device: {str(e)}"}

#get devices
@router.get("/")
async def get_devices():
    query = "SELECT * FROM Devices"
    try:
        users = await database.fetch_all(query=query)
        return {"device": [dict(user) for user in users]}
    except Exception as e:
        return {"message": "Failed to fetch device", "error": str(e)}

#get registration date from a user
@router.get("/registration_date")
async def get_registration_date(
    user_first_name: str,
    user_last_name: str,
    user_birth_date: date
):
    query = """
        SELECT D.registration_date
        FROM Devices D
        JOIN Users U ON D.user_id = U.user_id
        WHERE U.user_first_name = :user_first_name
            AND U.user_last_name = :user_last_name
            AND U.user_birth_date = :user_birth_date;
    """
    values = {
        "user_first_name": user_first_name,
        "user_last_name": user_last_name,
        "user_birth_date": user_birth_date
    }

    try:
        device = await database.fetch_one(query=query, values=values)
        if device:
            return {
                "registration_date": device["registration_date"]
            }
        else:
            return {"message": "User not found"}
    except Exception as e:
        return {"message": f"Failed to fetch user data: {str(e)}"}
