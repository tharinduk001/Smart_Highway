# Deployment Steps for Containers

## Step 01: MongoDB Setup
- MongoDB Image : mongo:6.0
1. **Pull MongoDB Image**  
   Use the following image: `mongo:6.0`.
2. **Set Up Shared Volume**  
   Map the shared volume: `/data/db`.
3. **Expose Ports**  
   Bind the container port `27017` to host port `27017`.

## Step 02: Backend Container Setup
- Backend Image : kalharacodes/highway-backend:latest
1. **Import Backend**  
   Ensure the backend service is imported into your project.
2. **Set Up Shared Volume**  
   Map the same shared volume: `/data/db`.
3. **Expose Ports**  
   Bind the container port `3000` to host port `3000`.
4. **Set Environment Variable**  
   Add the following environment variable:  
   ```bash
   MONGO_URI=mongodb://192.99.4.40:27017/rfid_logs

## Step 03: Frontend Container Setup
- Frontend Image : kalharacodes/highway-frontend:latest
1. **Import Frontend**  
Ensure the frontend service is imported into your project.
2. **Set Environment Variables**  
Define the following environment variables:  
- `WEBSOCKET_HOST` (IP address of backend)  
- `WEBSOCKET_PORT` (defautlt 3000)
3. **Expose Ports**  
Bind the container port `8080` to host port `80`.

## Step 04: Project is Live 
1. Now you can access the backend by http://BACKEND_IP:3000/validate-card/
2. You can access the frontend by http://FRONTEND_IP:8080
3. Happy Journey on Highway!