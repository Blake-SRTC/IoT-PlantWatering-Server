FROM node:18-alpine3.15

RUN mkdir -p /usr/src/app

WORKDIR /usr/src/app

COPY package*.json ./

RUN npm install

COPY . .

# EXPOSE 4050

CMD ["npm", "start"]
# CMD ["npm", "run", "dev"]