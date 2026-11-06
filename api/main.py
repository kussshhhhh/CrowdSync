from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
from typing import List, Set
import httpx

app = FastAPI(title="CrowdSync API")

CPP_ENGINE_URL = "https://crowdsync-engine.onrender.com"

class User(BaseModel):
    id: str
    lat: float
    lon: float
    interests: Set[str]

# In-memory mock database
db_users = {}

@app.post("/users/")
async def create_user(user: User):
    db_users[user.id] = user
    return {"status": "User created", "user_id": user.id}

@app.get("/discover/{user_id}")
async def discover_peers(user_id: str, radius_km: float = 10.0):
    if user_id not in db_users:
        raise HTTPException(status_code=404, detail="User not found")
    
    target_user = db_users[user_id]
    
    # 1. Filter by shared interests (Logic layer)
    interest_filtered_users = []
    for uid, user in db_users.items():
        if uid != user_id and target_user.interests.intersection(user.interests):
            interest_filtered_users.append({
                "id": uid,
                "lat": user.lat,
                "lon": user.lon
            })
            
    if not interest_filtered_users:
        return {"matches": []}

    # 2. Offload proximity math to C++ Engine
    payload = {
        "target": {"lat": target_user.lat, "lon": target_user.lon},
        "radius": radius_km,
        "users": interest_filtered_users
    }

    async with httpx.AsyncClient() as client:
        try:
            response = await client.post(CPP_ENGINE_URL, json=payload, timeout=5.0)
            response.raise_for_status()
            return response.json()
        except httpx.RequestError:
            raise HTTPException(status_code=503, detail="Geospatial engine unavailable")