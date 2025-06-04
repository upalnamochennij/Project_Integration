from fastapi import APIRouter
from database import database
from pydantic import BaseModel
from datetime import date

router = APIRouter()

class DeviceCreate(BaseModel):
    user_id: int
    registration_date: date

#create device this should be done by the product seller
@router.get("/create")
async def create_device(
    user_id: int,
    registration_date: date
):
    check_user_query = """
    SELECT 1 FROM Users WHERE user_id = :user_id
    """

    check_device_query = """
    SELECT 1 FROM Devices
    WHERE user_id = :user_id AND registration_date = :registration_date
    """

    insert_query = """
    INSERT INTO Devices (user_id, registration_date)
    VALUES (:user_id, :registration_date)
    """
    check_user_values = {
        "user_id": user_id,
    }

    check_device_values = {
        "user_id": user_id,
        "registration_date": registration_date,
    }

    insert_values = {
        "user_id": user_id,
        "registration_date": registration_date,
    }

    try:
        user_exists = await database.fetch_one(query=check_user_query, values=check_user_values)
        if not user_exists:
            return {"message": "User does not exist"}

        device_exists = await database.fetch_one(query=check_device_query, values=check_device_values)
        if device_exists:
            return {"message": "Device already exists for this user on this date"}

        await database.execute(query=insert_query, values=insert_values)
        return {"message": "Device created successfully"}

    except Exception as e:
        return {"message": f"Failed to create device: {str(e)}"}

#get all devices for debugging
@router.get("/get_all_devices")
async def get_devices():
    query = "SELECT * FROM Devices"
    try:
        users = await database.fetch_all(query=query)
        return {"device": [dict(user) for user in users]}
    except Exception as e:
        return {"message": "Failed to fetch device", "error": str(e)}

#get specific device/devices of a user
@router.get("/get_device")
async def get_device(
    user_id: int
):
    query = """
    SELECT * FROM Devices
    WHERE user_id = :user_id
    """

    values = {
        "user_id": user_id
    }
    try:
        users = await database.fetch_one(query=query, values=values)
        return {"device": [dict(user) for user in users]}
    except Exception as e:
        return {"message": "Failed to fetch device", "error": str(e)}
