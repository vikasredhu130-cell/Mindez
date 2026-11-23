# MindEase PBL Project

This package keeps your original C++ prototype intact and adds separate frontend/backend starter folders.

## Structure
- `frontend/` React + Vite website with Student and Doctor portal UI.
- `backend/api/` Express demo API.
- `backend/cpp-core/` your original C++ files, text data, and executable copied from the uploaded ZIP.

## Run frontend
1. `cd frontend`
2. `npm install`
3. `npm run dev`

## Run API
1. `cd backend/api`
2. `npm install`
3. `npm start`

Frontend currently uses local browser state for demo interactions; API is a separate starter and needs integration for a full connected application. The doctor portal is a demo role selector, not secure authentication. Do not deploy real wellbeing records with this prototype.

## Original C++ prototype
See `backend/cpp-core/README.txt` for compilation instructions. Original source files are preserved.
